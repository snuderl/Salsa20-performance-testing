using System;
using System.Security.Cryptography;
using System.Diagnostics;

namespace SalsaTest{

	public class MainClass{
			public static void Main(){
				TestRun(100,100000);
				TestRun(1000,10000);
				TestRun(10000,1000);




			}

			public static void TestRun(int messageSize, int runs){
				Salsa20 salsa = new SalsaTest.Salsa20();
				salsa.GenerateKey();
				salsa.GenerateIV();

				byte[] bytes = new byte[messageSize];
				RandomNumberGenerator rng = new RNGCryptoServiceProvider();
				rng.GetBytes(bytes);



				var encryptor = salsa.CreateEncryptor();

				Stopwatch stopWatch = new Stopwatch();

				stopWatch.Start();

				byte[] c;
				for(int i = 0;i<runs;i++){

					c = encryptor.TransformFinalBlock(bytes,0,bytes.Length);
				}
				stopWatch.Stop();
				Console.WriteLine("---- Encryption | " + (stopWatch.Elapsed.TotalMilliseconds / (runs)) + " seconds | " +messageSize + " bytes");

				c = encryptor.TransformFinalBlock(bytes,0,bytes.Length);
				var dev = salsa.CreateDecryptor();


				stopWatch.Reset();
				stopWatch.Start();
				for(int i = 0;i<runs;i++){
					byte[] m = dev.TransformFinalBlock(c,0,c.Length);
				}

				stopWatch.Stop();
				Console.WriteLine("---- Decryption | " + (stopWatch.Elapsed.TotalMilliseconds / (runs*1000)) + " seconds | " +messageSize + " bytes");

			}

	}
}