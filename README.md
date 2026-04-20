# 🖼️ Image Steganography using C (LSB Technique)

## 📌 Project Overview

This project implements **Image Steganography** using the **Least Significant Bit (LSB)** technique in C programming.

It allows users to **hide secret text inside BMP images** and later **extract hidden messages** without noticeable image distortion.

This project demonstrates **bit manipulation**, **file handling**, and **secure data hiding techniques**.

---

## 🚀 Features

✨ Encode secret message into BMP image  
✨ Decode hidden message from stego image  
✨ Uses LSB encoding technique  
✨ Supports BMP image format  
✨ Maintains original image quality  
✨ Modular structured implementation  

---

## 🛠️ Technologies Used

- Programming Language: **C**
- Concepts:
  - Bit Manipulation
  - File Handling
  - Binary File Processing
  - LSB Encoding
  - Modular Programming

---

## 📂 Project Structure

 Image-Steganography/
│
├── Encode.c → Encoding functionality
├── decode.c → Decoding functionality
├── Common.C → Shared functions
├── Types.c → Data structures
├── encode.h → Header file
├── Test_encode.c → Test execution file
├── Beautiful.bmp → Input image
├── Stego.bmp → Encoded image
├── Secret.txt → Secret message file
├── output.txt → Decoded message output
├── README.md



## ▶️ How to Compile

```bash
gcc Encode.c decode.c Common.C Types.c Test_encode.c -o stego

How to Run
Encode Message
./stego encode Beautiful.bmp Secret.txt Stego.bmp
Decode Message
./stego decode Stego.bmp output.txt


Sample Output
Encoding Successful
Secret message hidden successfully.

Decoding Successful
Secret message extracted successfully.

Learning Outcomes

✔ Learned LSB encoding technique
✔ Implemented bit-level manipulation
✔ Practiced binary file handling
✔ Developed modular structured programs
✔ Understood secure data hiding methods

Future Enhancements

🔹 Support multiple image formats
🔹 Add encryption before encoding
🔹 Improve performance
🔹 Develop GUI interface
