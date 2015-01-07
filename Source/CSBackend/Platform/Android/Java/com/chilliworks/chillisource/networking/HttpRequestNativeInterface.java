/**
 * HttpRequestNativeInterface.java
 * Chilli Source
 * Created by Ian Copland on 15/08/2011.
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

import java.io.EOFException;
import java.net.HttpURLConnection;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.UnknownHostException;

import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedInputStream;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.X509TrustManager;

import com.chilliworks.chillisource.core.DynamicByteBuffer;
import com.chilliworks.chillisource.core.Logging;
import com.chilliworks.chillisource.networking.InsecureHostnameVerifier;
import com.chilliworks.chillisource.networking.InsecureTrustManager;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;

public class HttpRequestNativeInterface 
{
	static final int kdwDataBlockSize = 128;
	
	static final int k_readTimeoutMilliSecs = 60000;
	
	private static Activity msActivity;
	private static X509TrustManager[] msTrustManagers;
	private static HostnameVerifier msHostnameVerifier;
	
	/**
	 * Called when the max buffer size is exceeded by an http request causing it
	 * to flush the current buffer contents to the application
	 * 
	 * @author S Downie
	 * 
	 * @param in_data - The partial response data that has been flushed
	 * @param in_dataLength - The length of the partial response data
	 * @param in_responseCode - The http reponse code
	 * @param in_requestId - The id of this request
	 */
	public static native void OnBufferFlushed(byte[] in_data, int in_dataLength, int in_responseCode, int in_requestId);
	
	//-------------------------------------------------------------------
	/// Setup
	///
	/// Initialises the native interface.
	///
	/// @param The applications activity
	//-------------------------------------------------------------------
	public static void Setup(Activity inActivity)
	{
		msActivity = inActivity;
		msTrustManagers = new X509TrustManager[] {new InsecureTrustManager()};
		msHostnameVerifier = new InsecureHostnameVerifier();
		
		HttpURLConnection.setFollowRedirects(true);
	}
	//-------------------------------------------------------------------
	/// Http Request with headers
	///
	/// Method accessible from native for processing a HTTP request
	/// including additional headers.
	///
	/// @param the url
	/// @param Whether or not its a post request
	/// @param Array of key strings for headers
	/// @param Array of value strings for headers
	/// @param The post body. Not used if inbIsPost is false
	/// @param The connection timeout in seconds
	/// @param Request Id
	/// @param The result length. This is stored as an array in order to
	///		   pass by reference. 
	/// @param The result code. This is stored as an array in order to
	///		   pass by reference.
	/// @param The http response code. This is stored as an array in order 
	///		   to pass by reference.
	/// @return The response as a byte array.
	//-------------------------------------------------------------------
	public static byte[] HttpRequestWithHeaders(String instrUrl, boolean inbIsPost,
									 			String[] inastrHeaderKeys, String[] inastrHeaderValues, String instrBody, int in_timeout, int in_maxBufferSize, int in_requestId,
									 			int[] outadwResultLength, int[] outadwResultCode, int[] outadwHttpResponseCode)
	{
		int connectionTimeoutMs = in_timeout * 1000;
		
		boolean bRetry = false;

		outadwResultLength[0] = 0;
		outadwResultCode[0] = 0; //HRRC_SUCCESS
		outadwHttpResponseCode[0] = -1;
		byte[] abyOutputData = null;
		do
		{
			bRetry = false;
			outadwResultLength[0] = 0;
			outadwResultCode[0] = 0; //HRRC_SUCCESS
			outadwHttpResponseCode[0] = -1;
			abyOutputData = null;

			try
			{	
				System.setProperty("http.keepAlive", "true");

				// In 2.2 and below keep-alive is bugged, so force it off.
				// Otherwise keep-alives are enabled by default
				if (Build.VERSION.SDK_INT < Build.VERSION_CODES.GINGERBREAD)
				{
					System.setProperty("http.keepAlive", "false");
				}

				HttpURLConnection urlConnection = (HttpURLConnection) new URL(instrUrl).openConnection();

				try 
				{	        	
					urlConnection.setReadTimeout(k_readTimeoutMilliSecs);
					urlConnection.setConnectTimeout(connectionTimeoutMs);

					//if the protocol is HTTPS then set that up.
					if (instrUrl.startsWith("https") == true)
					{
						SSLContext context = SSLContext.getInstance("TLS");
						context.init(null, msTrustManagers, null);
						((HttpsURLConnection)urlConnection).setSSLSocketFactory(context.getSocketFactory());
						((HttpsURLConnection)urlConnection).setHostnameVerifier(msHostnameVerifier);
					}

					//If we are posting we need to set up the post before opening the connection
					if (inbIsPost == true)
					{
						urlConnection.setDoOutput(true);
						urlConnection.setFixedLengthStreamingMode(instrBody.getBytes().length);
					}

					//--Set headers
					if((inastrHeaderKeys != null) && (inastrHeaderValues != null) && (inastrHeaderKeys.length == inastrHeaderValues.length))
					{
						// Headers are passed in as a key/value strings array
						for(int i = 0; i < inastrHeaderKeys.length; i++)
						{	
							urlConnection.setRequestProperty(inastrHeaderKeys[i], inastrHeaderValues[i]);
						}
					}

					urlConnection.connect();

					//if this is a post request then set the body
					if (inbIsPost == true)
					{
						byte[] abyOutData = instrBody.getBytes();
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
						outadwHttpResponseCode[0] = 503;
					}
					else 
					{
						// Get the response (either by InputStream or ErrorStream)
						abyOutputData = ReadStream(dwResponseCode, in_maxBufferSize, in_requestId, urlConnection, outadwResultLength);
					}
				}
				catch (UnknownHostException eUnknownHostException)
				{
					outadwResultCode[0] = 2; //HRRC_TIMEOUT
					Logging.logError(eUnknownHostException.getMessage());
				}
				catch (SocketTimeoutException timeoutException)
				{
					outadwResultCode[0] = 2; //HRRC_TIMEOUT
					Logging.logError(timeoutException.getMessage());
				}
				catch (EOFException eofException)
				{
					outadwResultCode[0] = 2; //HRRC_TIMEOUT
					Logging.logError(eofException.getMessage());
				}
				catch (Exception eConnectionException)
				{
					outadwResultCode[0] = 1;//HRRC_COULDNOTCONNECT
					Logging.logError(eConnectionException.getMessage());
				}
				finally 
				{
					//get the response code
					if (outadwHttpResponseCode[0] == -1 && outadwResultCode[0] != 2)
					{
						outadwHttpResponseCode[0] = urlConnection.getResponseCode();
					}

					//Disconnect the url connection. Note, this does not necessarily close the connection, possibly simply returning it to a pool of open connections for reuse.
					urlConnection.disconnect();
				}
			}
			catch(Exception eUrlException)
			{
				if(eUrlException.getMessage().contains("ECONNRESET"))
				{
					bRetry = true;
					continue;
				}

				outadwResultCode[0] = 1;//HRRC_COULDNOTCONNECT
				Logging.logError(eUrlException.getMessage());
			}

		} while(bRetry);
		return abyOutputData;
	}
	
	private static byte[] ReadStream(int indwResponseCode, int in_maxBufferSize, int in_requestId, HttpURLConnection inurlConnection, int[] outadwResultLength) throws IOException
	{
		DynamicByteBuffer byteContainer = new DynamicByteBuffer(kdwDataBlockSize);
    	byte[] byDataBlock = new byte[kdwDataBlockSize];
    	byte[] abyOutputData = null;
		
    	int dwAmountRead = 0;
    	int currentBufferSize = 0;
    	InputStream reader = null;
    	try
    	{
    		// Check for http 200 or 301
    		if (indwResponseCode == HttpURLConnection.HTTP_OK || indwResponseCode == HttpURLConnection.HTTP_MOVED_PERM)
    		{
    			dwAmountRead = inurlConnection.getInputStream().read(byDataBlock);
    			reader = new BufferedInputStream(inurlConnection.getInputStream());
    		}
    		// Check for http 500
    		else if(indwResponseCode == HttpsURLConnection.HTTP_INTERNAL_ERROR || indwResponseCode == HttpsURLConnection.HTTP_UNAVAILABLE)
    		{
    			dwAmountRead = inurlConnection.getErrorStream().read(byDataBlock);
    			reader = new BufferedInputStream(inurlConnection.getErrorStream());
    		}

    		if(reader != null)
    		{			
    			// Keep reading until the end has been found
    			while (dwAmountRead != -1)
    			{
    				if(in_maxBufferSize > 0 && currentBufferSize + dwAmountRead >= in_maxBufferSize)
    				{
    					//Flush the buffer
    					OnBufferFlushed(byteContainer.getInternalBuffer(), byteContainer.getByteCount(), indwResponseCode, in_requestId);
    					currentBufferSize = 0;
    					byteContainer.clear();
    				}
    				
    				byteContainer.appendBytes(byDataBlock, dwAmountRead);
    				currentBufferSize += dwAmountRead;
    				dwAmountRead = reader.read(byDataBlock);
    			}

    			reader.close();

    			abyOutputData = byteContainer.getInternalBuffer();
    			outadwResultLength[0] = byteContainer.getByteCount();
    		}
    		return abyOutputData;
    	}
		catch(IOException eIOException)
		{
			if(reader != null)
				reader.close();
			
			return null;
		}
	}
	//-------------------------------------------------------------------
	/// Is Connected
	///
	/// method accessible from native to test whether or not the device is
	/// connected to the Internet.
	///
	/// @return whether or not the device is connected.
	//-------------------------------------------------------------------
	public static boolean IsConnected()
	{
		ConnectivityManager cm = (ConnectivityManager) msActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo netInfo = cm.getActiveNetworkInfo();
	    if (netInfo != null && netInfo.isConnected()) 
	    {
	        return true;
	    }
	    return false;
	}
}
