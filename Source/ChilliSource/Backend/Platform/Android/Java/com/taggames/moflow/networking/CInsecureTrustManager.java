package com.taggames.moflow.networking;

import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.ssl.X509TrustManager;

//===================================================================
/// Insecure Trust Manager
///
/// a trust manager than "trusts" all certificates.
//===================================================================
public class CInsecureTrustManager implements X509TrustManager
{
	@Override
	public void checkClientTrusted(X509Certificate[] arg0, String arg1) throws CertificateException 
	{}

	@Override
	public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException 
	{}

	@Override
	public X509Certificate[] getAcceptedIssuers() 
	{
		return null;
	}
}