package com.taggames.moflow.networking;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.SSLSession;

//===================================================================
/// Insecure Hostname Verifier
///
/// a host name verifier than always confirms the hostname
//===================================================================
public class CInsecureHostnameVerifier implements HostnameVerifier 
{
	@Override
	public boolean verify(String hostname, SSLSession session) 
	{
		return true;
	}
}
