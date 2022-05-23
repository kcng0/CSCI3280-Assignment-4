dct.cpp
Enhanced Features:	IDCT
			Efficient DCT
			Efficient IDCT
Compile: Same as requirement, ie.	via Visual Studio command prompt (Tools
					Command Prompt) with the command line:  C:\> cl dct.cpp bmp.cpp

Running: Same as requirement, ie.	C:\> dct.exe <img_path> <apply_idct>


dct_extra.cpp
Enhanced Features:	Efficient IDCT
			Reconstruct image from txt file
Compile:	via Visual Studio command prompt Tools
		Command Prompt) with command line C:\>cl dct_extra.cpp bmp.cpp
Running:	C:\>dct_extra.exe <txt_path> <output_name>

Description:	The dct_extra.cpp file converts the text file that stores quantized frequency coefficient array
		and output by dct.cpp. The <txt_path> indicates the text file that stores quantized array. And
		the <output_name> is the bmp file name to establish the rescontructed image.