/*
 *  CUnicodeUtil.java
 *  ToolUtils
 *
 *  Created by Robert Henning on 13/06/2012.
 *  Source taken from source by Ryan Fernandes
 *  URL: http://tripoverit.blogspot.co.uk/2007/04/javas-utf-8-and-unicode-writing-is.html
 *  
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

package com.taggames.toolutils;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PushbackInputStream;
import java.io.UnsupportedEncodingException;
import java.io.Writer;

public class CUnicodeUtil
{
	public static byte[] convert(byte[] bytes, String encout) throws Exception
	{
		// Workaround for bug that will not be fixed by SUN
		// http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4508058
		UnicodeInputStream uis = new UnicodeInputStream(new ByteArrayInputStream(bytes), "ASCII");
		boolean unicodeOutputReqd = (getBOM(encout) != null) ? true : false;
		String enc = uis.getEncoding();
		String BOM = getBOM(enc); // get the BOM of the inputstream

		if (BOM == null) {
			// inputstream looks like ascii...
			// create a BOM based on the outputstream
			BOM = getBOM(encout);
		}
		uis.close();

		ByteArrayOutputStream out = new ByteArrayOutputStream();
		BufferedReader br = new BufferedReader(new InputStreamReader(new ByteArrayInputStream(bytes, uis.getBOMOffset(), bytes.length), enc));
		Writer w = new BufferedWriter(new OutputStreamWriter(out, encout));

		// dont write a BOM for ascii(out) as the OutputStreamWriter
		// will not process it correctly.
		if (BOM != null && unicodeOutputReqd) {
			w.write(BOM);
		}

		char[] buffer = new char[4096];
		int len;
		while ((len = br.read(buffer)) != -1) {
			w.write(buffer, 0, len);
		}

		br.close(); // Close the input.
		w.close(); // Flush and close output.
		return out.toByteArray();
	}

	public static String getBOM(String enc) throws UnsupportedEncodingException {
		if ("UTF-8".equals(enc)) {
			byte[] bom = new byte[3];
			bom[0] = (byte) 0xEF;
			bom[1] = (byte) 0xBB;
			bom[2] = (byte) 0xBF;
			return new String(bom, enc);
		} else if ("UTF-16BE".equals(enc)) {
			byte[] bom = new byte[2];
			bom[0] = (byte) 0xFE;
			bom[1] = (byte) 0xFF;
			return new String(bom, enc);
		} else if ("UTF-16LE".equals(enc)) {
			byte[] bom = new byte[2];
			bom[0] = (byte) 0xFF;
			bom[1] = (byte) 0xFE;
			return new String(bom, enc);
		} else if ("UTF-32BE".equals(enc)) {
			byte[] bom = new byte[4];
			bom[0] = (byte) 0x00;
			bom[1] = (byte) 0x00;
			bom[2] = (byte) 0xFE;
			bom[3] = (byte) 0xFF;
			return new String(bom, enc);
		} else if ("UTF-32LE".equals(enc)) {
			byte[] bom = new byte[4];
			bom[0] = (byte) 0x00;
			bom[1] = (byte) 0x00;
			bom[2] = (byte) 0xFF;
			bom[3] = (byte) 0xFE;
			return new String(bom, enc);
		} else {
			return null;
		}

	}

	public static class UnicodeInputStream extends InputStream {
		private PushbackInputStream internalIn;

		private boolean isInited = false;

		private int BOMOffset = -1;

		private String defaultEnc;

		private String encoding;

		public static final int BOM_SIZE = 4;

		public UnicodeInputStream(InputStream in, String defaultEnc) {
			internalIn = new PushbackInputStream(in, BOM_SIZE);
			this.defaultEnc = defaultEnc;
		}

		public String getDefaultEncoding() {
			return defaultEnc;
		}

		public String getEncoding() {
			if (!isInited) {
				try {
					init();
				} catch (IOException ex) {
					IllegalStateException ise = new IllegalStateException("Init method failed.");
					ise.initCause(ise);
					throw ise;
				}
			}
			return encoding;
		}

		/**
		 * Read-ahead four bytes and check for BOM marks. Extra bytes are unread
		 * back to the stream, only BOM bytes are skipped.
		 */
		protected void init() throws IOException {
			if (isInited)
				return;

			byte bom[] = new byte[BOM_SIZE];
			int n, unread;
			n = internalIn.read(bom, 0, bom.length);

			if ((bom[0] == (byte) 0x00) && (bom[1] == (byte) 0x00) && (bom[2] == (byte) 0xFE) && (bom[3] == (byte) 0xFF)) {
				encoding = "UTF-32BE";
				unread = n - 4;
			} else if ((bom[0] == (byte) 0xFF) && (bom[1] == (byte) 0xFE) && (bom[2] == (byte) 0x00) && (bom[3] == (byte) 0x00)) {
				encoding = "UTF-32LE";
				unread = n - 4;
			} else if ((bom[0] == (byte) 0xEF) && (bom[1] == (byte) 0xBB) && (bom[2] == (byte) 0xBF)) {
				encoding = "UTF-8";
				unread = n - 3;
			} else if ((bom[0] == (byte) 0xFE) && (bom[1] == (byte) 0xFF)) {
				encoding = "UTF-16BE";
				unread = n - 2;
			} else if ((bom[0] == (byte) 0xFF) && (bom[1] == (byte) 0xFE)) {
				encoding = "UTF-16LE";
				unread = n - 2;
			} else {
				// Unicode BOM mark not found, unread all bytes
				encoding = defaultEnc;
				unread = n;
			}
			BOMOffset = BOM_SIZE - unread;
			if (unread > 0)
				internalIn.unread(bom, (n - unread), unread);

			isInited = true;
		}

		public void close() throws IOException {
			// init();
			isInited = true;
			internalIn.close();
		}

		public int read() throws IOException {
			// init();
			isInited = true;
			return internalIn.read();
		}

		public int getBOMOffset() {
			return BOMOffset;
		}
	}

}