/**
 * HttpRequest.java
 * Chilli Source
 * Created by HMcLaughlin on 31/07/2015.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2011 Tag Games Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.chilliworks.chillisource.networking;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;

import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.DynamicByteBuffer;
import com.chilliworks.chillisource.core.Logging;
import com.chilliworks.chillisource.core.NativePointer;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.UnknownHostException;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.X509TrustManager;

/**
 * Class used to handle Java http functionality
 *
 * @author HMcLaughlin
 */
public final class HttpRequest
{
    private final int k_dataBlockSize = 128;
    private final int k_readTimeoutMilliSecs = 60000;

    private X509TrustManager[] m_trustManagers;
    private HostnameVerifier m_hostnameVerifier;

    private NativePointer m_owningNativeObject;

    final private int k_success  = 0;
    final private int k_failed   = 1;
    final private int k_timeout  = 2;
    final private int k_flushed  = 3;

    private volatile int m_downloadedDataSize = 0;
    private int m_downloadSize = 0;

    /**
     * Called when the max buffer size is exceeded by an http request causing it
     * to flush the current buffer contents to the application
     *
     * @author HMcLaughlin
     *
     * @param in_data - The partial response data that has been flushed
     * @param in_dataLength - The length of the partial response data
     * @param in_responseCode - The http reponse code
     * @param in_objectPointer - The pointer information of the owning native object
     */
    public static native void OnBufferFlushed(byte[] in_data, int in_dataLength, int in_responseCode, NativePointer in_objectPointer);
    /**
     * Called when the connection has finished sending data
     *
     * @author HMcLaughlin
     *
     * @param in_resultCode - Result code
     * @param in_data - The partial response data that has been flushed
     * @param in_dataLength - The length of the partial response data
     * @param in_responseCode - The http reponse code
     * @param in_objectPointer - The pointer information of the owning native object
     */
    public static native void OnComplete(int in_resultCode, byte[] in_data, int in_dataLength, int in_responseCode, NativePointer in_objectPointer);

    public HttpRequest(NativePointer in_owningNativeObject)
    {
        m_owningNativeObject = in_owningNativeObject;

        m_trustManagers = new X509TrustManager[] {new InsecureTrustManager()};
        m_hostnameVerifier = new InsecureHostnameVerifier();

        HttpURLConnection.setFollowRedirects(true);
    }
    /**
     * Http Request with headers
     *
     * Method accessible from native for processing a HTTP request
     * including additional headers.
     *
     * @author HMcLaughlin
     *
     * @param in_url - The url
     * @param in_isPost - Whether or not its a post request
     * @param in_headerKeys - Array of key strings for headers
     * @param in_headerValues - Array of value strings for headers
     * @param in_body - The post body. Not used if in_isPost is false
     * @param in_timeout - The connection timeout in seconds
     * @param in_maxBufferSize - Download bytes before flushing
     */
    public void makeHttpRequestWithHeaders(final String in_url, final boolean in_isPost, final String[] in_headerKeys,
                                       final String[] in_headerValues, final String in_body, final int in_timeout, final int in_maxBufferSize)
    {
        //Run the polling of the connection in a background thread
        Runnable httpTask =  new Runnable()
        {
            @Override
            public void run()
            {
                int connectionTimeoutMs = in_timeout * 1000;

                boolean retry = false;

                do
                {
                    retry = false;

                    try
                    {
                        System.setProperty("http.keepAlive", "true");

                        // In 2.2 and below keep-alive is bugged, so force it off.
                        // Otherwise keep-alives are enabled by default
                        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.GINGERBREAD)
                        {
                            System.setProperty("http.keepAlive", "false");
                        }

                        HttpURLConnection urlConnection = (HttpURLConnection) new URL(in_url).openConnection();

                        try
                        {
                            urlConnection.setReadTimeout(k_readTimeoutMilliSecs);
                            urlConnection.setConnectTimeout(connectionTimeoutMs);

                            //if the protocol is HTTPS then set that up.
                            if (in_url.startsWith("https") == true)
                            {
                                SSLContext context = SSLContext.getInstance("TLS");
                                context.init(null, m_trustManagers, null);
                                ((HttpsURLConnection) urlConnection).setSSLSocketFactory(context.getSocketFactory());
                                ((HttpsURLConnection) urlConnection).setHostnameVerifier(m_hostnameVerifier);
                            }

                            //If we are posting we need to set up the post before opening the connection
                            if (in_isPost == true)
                            {
                                urlConnection.setDoOutput(true);
                                urlConnection.setFixedLengthStreamingMode(in_body.getBytes().length);
                            }

                            //--Set headers
                            if ((in_headerKeys != null) && (in_headerValues != null) && (in_headerKeys.length == in_headerValues.length))
                            {
                                // Headers are passed in as a key/value strings array
                                for (int i = 0; i < in_headerKeys.length; i++)
                                {
                                    urlConnection.setRequestProperty(in_headerKeys[i], in_headerValues[i]);
                                }
                            }

                            urlConnection.connect();
                            //if this is a post request then set the body
                            if (in_isPost == true)
                            {
                                byte[] abyOutData = in_body.getBytes();
                                OutputStream writer = new BufferedOutputStream(urlConnection.getOutputStream());
                                writer.write(abyOutData);
                                writer.flush();
                                writer.close();
                            }

                            int dwResponseCode = urlConnection.getResponseCode();
                            if (dwResponseCode == -1)
                            {
                                //If we've got this far and the response code is -1 then we've encountered the dreaded 2.2 bug!
                                //In order to get around this, we should return a fake 503 error to force the client to try the call again.
                                onFailed(k_failed, 503);
                            }
                            else
                            {
                                m_downloadSize = urlConnection.getContentLength();

                                // Get the response (either by InputStream or ErrorStream)
                                readStream(dwResponseCode, in_maxBufferSize, urlConnection);
                            }
                        }
                        catch (UnknownHostException eUnknownHostException)
                        {
                            onFailed(k_timeout, urlConnection.getResponseCode());
                            Logging.logError(eUnknownHostException.getMessage());
                        }
                        catch (SocketTimeoutException timeoutException)
                        {
                            onFailed(k_timeout, urlConnection.getResponseCode());
                            Logging.logError(timeoutException.getMessage());
                        }
                        catch (EOFException eofException)
                        {
                            onFailed(k_timeout, urlConnection.getResponseCode());
                            Logging.logError(eofException.getMessage());
                        }
                        catch (Exception eConnectionException)
                        {
                            onFailed(k_failed, urlConnection.getResponseCode());
                            Logging.logError(eConnectionException.getMessage());
                        }
                        finally
                        {
                            //Disconnect the url connection. Note, this does not necessarily close the connection, possibly simply returning it to a pool of open connections for reuse.
                            urlConnection.disconnect();
                        }
                    }
                    catch (Exception eUrlException)
                    {
                        if (eUrlException.getMessage().contains("ECONNRESET"))
                        {
                            retry = true;
                            continue;
                        }

                        onFailed(k_failed, 404);
                        Logging.logError(eUrlException.getMessage());
                    }

                } while (retry);
            }
        };

        (new Thread(httpTask)).start();
    }
    /**
     * Reads data from the url connection until complete/failed
     *
     * @author HMcLaughlin
     *
     * @param in_responseCode - Reponse Code
     * @param in_maxBufferSize - Max buffer size
     * @param in_connection - URL Connection
     */
    private void readStream(int in_responseCode, int in_maxBufferSize, HttpURLConnection in_connection) throws IOException
    {
        DynamicByteBuffer byteContainer = new DynamicByteBuffer(k_dataBlockSize);
        byte[] byDataBlock = new byte[k_dataBlockSize];
        byte[] abyOutputData = null;

        int dwAmountRead = 0;
        int currentBufferSize = 0;
        InputStream reader = null;
        try
        {
            // Check for http 200 or 301
            if (in_responseCode == HttpURLConnection.HTTP_OK || in_responseCode == HttpURLConnection.HTTP_MOVED_PERM)
            {
                dwAmountRead = in_connection.getInputStream().read(byDataBlock);
                reader = new BufferedInputStream(in_connection.getInputStream());
            }
            // Check for http 500
            else if(in_responseCode == HttpsURLConnection.HTTP_INTERNAL_ERROR || in_responseCode == HttpsURLConnection.HTTP_UNAVAILABLE)
            {
                dwAmountRead = in_connection.getErrorStream().read(byDataBlock);
                reader = new BufferedInputStream(in_connection.getErrorStream());
            }

            if(reader != null)
            {
                // Keep reading until the end has been found
                while (dwAmountRead != -1)
                {
                    if(in_maxBufferSize > 0 && currentBufferSize + dwAmountRead >= in_maxBufferSize)
                    {
                        //Flush the buffer
                        OnBufferFlushed(byteContainer.getInternalBuffer(), byteContainer.getByteCount(), in_responseCode, m_owningNativeObject);
                        currentBufferSize = 0;
                        byteContainer.clear();
                    }

                    addDownloadProgress(dwAmountRead);

                    byteContainer.appendBytes(byDataBlock, dwAmountRead);
                    currentBufferSize += dwAmountRead;
                    dwAmountRead = reader.read(byDataBlock);
                }

                reader.close();
            }

            OnComplete(k_success, byteContainer.getInternalBuffer(), byteContainer.getByteCount(), in_responseCode,  m_owningNativeObject);
        }
        catch(IOException eIOException)
        {
            if(reader != null)
                reader.close();

            onFailed(k_failed, in_responseCode);
        }
    }
    /**
     * @author HMcLaughlin
     *
     * @return Total size of the download, can be 0
     */
    public long getExpectedTotalSize()
    {
        return m_downloadSize;
    }
    /**
     * @author HMcLaughlin
     *
     * @return Total size of the download, can be 0
     */
    public long getCurrentSize()
    {
        return m_downloadedDataSize;
    }
    /**
     * Called when the connection fails for any reason
     *
     * @author HMcLaughlin
     *
     * @param in_result - Result Code
     * @param in_respose - Response Code
     */
    private void onFailed(int in_result, int in_respose)
    {
        OnComplete(in_result, null, 0, in_respose,  m_owningNativeObject);
    }
    /**
     * Used to safely append downloaded bytes to the total downloaded data
     *
     * @author HMcLaughlin
     *
     * @param in_downloadProgressBytes - Downloaded data size
     */
    synchronized private void addDownloadProgress(int in_downloadProgressBytes)
    {
        m_downloadedDataSize += in_downloadProgressBytes;
    }
    //-------------------------------------------------------------------
    /// Is Connected
    ///
    /// method accessible from native to test whether or not the device is
    /// connected to the Internet.
    ///
    /// @return whether or not the device is connected.
    //-------------------------------------------------------------------
    public boolean IsConnected()
    {
        Activity activity = CSApplication.get().getActivity();
        ConnectivityManager cm = (ConnectivityManager) activity.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo netInfo = cm.getActiveNetworkInfo();
        if (netInfo != null && netInfo.isConnected())
        {
            return true;
        }
        return false;
    }
}
