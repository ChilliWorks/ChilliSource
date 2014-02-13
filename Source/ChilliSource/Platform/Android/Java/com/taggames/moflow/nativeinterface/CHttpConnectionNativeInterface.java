package com.taggames.moflow.nativeinterface;

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

import com.taggames.moflow.networking.CInsecureHostnameVerifier;
import com.taggames.moflow.networking.CInsecureTrustManager;
import com.taggames.moflow.networking.DynamicByteContainer;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.util.Log;


public class CHttpConnectionNativeInterface 
{
	static final int kdwTimeoutInMilliseconds = 15000;
	static final int kdwDataBlockSize = 128;
	
	private static Activity msActivity;
	private static X509TrustManager[] msTrustManagers;
	private static HostnameVerifier msHostnameVerifier;
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
		msTrustManagers = new X509TrustManager[] {new CInsecureTrustManager()};
		msHostnameVerifier = new CInsecureHostnameVerifier();
	}
	//-------------------------------------------------------------------
	/// Http Request
	///
	/// method accessible from native for processing a HTTP request.
	///
	/// @param the url
	/// @param Whether or not its a post request
	/// @param The post body. Not used if inbIsPost is false
	/// @param The result length. This is stored as an array in order to
	///		   pass by reference. 
	/// @param The result code. This is stored as an array in order to
	///		   pass by reference.
	/// @param The http response code. This is stored as an array in order 
	///		   to pass by reference.
	/// @return The response as a byte array.
	//-------------------------------------------------------------------
	public static byte[] HttpRequest(String instrUrl, boolean inbIsPost, String instrBody,
									 int[] outadwResultLength, String[] outstrRedirectionLocation, int[] outadwResultCode, int[] outadwHttpResponseCode)
	{
		return HttpRequestWithHeaders(instrUrl, inbIsPost, null, null, instrBody, outadwResultLength, outstrRedirectionLocation, outadwResultCode, outadwHttpResponseCode, true);
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
	/// @param The result length. This is stored as an array in order to
	///		   pass by reference. 
	/// @param The result code. This is stored as an array in order to
	///		   pass by reference.
	/// @param The http response code. This is stored as an array in order 
	///		   to pass by reference.
	/// @param Whether or not to establish a persistent connection
	/// @return The response as a byte array.
	//-------------------------------------------------------------------
	public static byte[] HttpRequestWithHeaders(String instrUrl, boolean inbIsPost,
									 			String[] inastrHeaderKeys, String[] inastrHeaderValues, String instrBody,
									 			int[] outadwResultLength, String[] outstrRedirectionLocation, int[] outadwResultCode, int[] outadwHttpResponseCode,
									 			boolean inbKeepAlive)
	{
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
			if (!inbKeepAlive || (Build.VERSION.SDK_INT < Build.VERSION_CODES.GINGERBREAD))
			{
				System.setProperty("http.keepAlive", "false");
			}

			HttpURLConnection urlConnection = (HttpURLConnection) new URL(instrUrl).openConnection();
			
	        try 
	        {	        	
	        	HttpURLConnection.setFollowRedirects(false);
	        	urlConnection.setReadTimeout(kdwTimeoutInMilliseconds);
	        	urlConnection.setConnectTimeout(kdwTimeoutInMilliseconds);
	        	
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
	        		abyOutputData = ReadStream(dwResponseCode, urlConnection, outadwResultLength);
	        	}
	        	
	        	 if(dwResponseCode == HttpsURLConnection.HTTP_MOVED_TEMP)
	         	{
	         		String strLocation = urlConnection.getHeaderField("Location");
	         		outstrRedirectionLocation[0] = strLocation;
	         	}
	        	
	        }
	        catch (UnknownHostException eUnknownHostException)
	        {
	        	outadwResultCode[0] = 2; //HRRC_TIMEOUT
	        	Log.e("MoFlowHTTP", eUnknownHostException.getMessage());
	        }
	        catch (SocketTimeoutException timeoutException)
	        {
	        	outadwResultCode[0] = 2; //HRRC_TIMEOUT
	        	Log.e("MoFlowHTTP", timeoutException.getMessage());
	        }
	        catch (EOFException eofException)
	        {
	        	outadwResultCode[0] = 2; //HRRC_TIMEOUT
	        	Log.e("MoFlowHTTP", eofException.getMessage());
	        }
	        catch (Exception eConnectionException)
	        {
	        	outadwResultCode[0] = 1;//HRRC_COULDNOTCONNECT
	        	Log.e("MoFlowHTTP", eConnectionException.getMessage());
	        }
	        finally 
	        {
	        	//get the response code
	        	if (outadwHttpResponseCode[0] == -1 && outadwResultCode[0] != 2)
	        	{
	        		outadwHttpResponseCode[0] = urlConnection.getResponseCode();
	        	}
	        	
	    		if(outadwHttpResponseCode[0] == -1)
	    		{
	    			int b = 5;
	    			b = b + 2;
	    		}
	        	
	        	//Disconnect the url connection. Note, this does not necessarily close the connection, possibly simply returning it to a pool of open connections for reuse.
	        	urlConnection.disconnect();
	        }
		}
		catch(Exception eUrlException)
		{
    		if(outadwHttpResponseCode[0] == -1)
    		{
    			int b = 5;
    			b = b + 2;
    		}
    		
    		if(eUrlException.getMessage().contains("ECONNRESET"))
    		{
    			bRetry = true;
    			continue;
    		}
    		
			outadwResultCode[0] = 1;//HRRC_COULDNOTCONNECT
			Log.e("MoFlowHTTP", eUrlException.getMessage());
		}
		
		if(outadwHttpResponseCode[0] == -1)
		{
			int b = 5;
			b = b + 2;
		}
		} while(bRetry);
		return abyOutputData;
	}
	
	private static byte[] ReadStream(int indwResponseCode, HttpURLConnection inurlConnection, int[] outadwResultLength) throws IOException
	{
		DynamicByteContainer byteContainer = new DynamicByteContainer(kdwDataBlockSize);
    	byte[] byDataBlock = new byte[kdwDataBlockSize];
    	byte[] abyOutputData = null;
		
    	int dwAmountRead = 0;
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
				byteContainer.AddBytes(byDataBlock, 0, dwAmountRead);
				dwAmountRead = reader.read(byDataBlock);
			}
			
			reader.close();
			
			abyOutputData = byteContainer.GetBytes();
			outadwResultLength[0] = byteContainer.GetSize();
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
