#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

/**
   Processes a pixel by forming the negative.
   @param blue the blue value of the pixel
   @param green the green value of the pixel
   @param red the red value of the pixel
*/
void process(int& blue, int& green, int& red, int reBlue, int reGreen, int reRed)
{
   const int haha = 110;
   const int damn = -110;
   int Nred = red - 19;
   int Ngreen = green - 255;
   int Nblue = blue - 9;
   if ((Nred <= haha && Nred >= damn) && (Ngreen <= haha && Ngreen >= damn) && (Nblue <= haha && Nblue >= damn))
   {
      red = reRed;
      green = reGreen;
      blue = reBlue;
   }
}

/**
   Gets an integer from a binary stream.
   @param stream the stream
   @param offset the offset at which to read the integer
   @return the integer starting at the given offset
*/
int get_int(fstream& stream, int offset)
{
   stream.seekg(offset);
   int result = 0;
   int base = 1;
   for (int i = 0; i < 4; i++)
   {
      result = result + stream.get() * base;
      base = base * 256;
   }
   return result;
}

int main()
{
   fstream stream;  //original photo
   fstream ostream; //copy photo
   fstream bstream; //beach photo
   fstream pstream;
   
    stream.open("greenScreen.bmp", ios::in | ios::out | ios::binary);
   ostream.open("output.bmp", ios::in | ios::out | ios::binary);
   bstream.open("beachh.bmp", ios::in | ios::out | ios::binary);
   pstream.open("damn.bmp", ios::in | ios::out | ios::binary);


   int file_size = get_int(stream, 2); // Get the image dimensions
   int start = get_int(stream, 10);
   int width = get_int(stream, 18);
   int height = get_int(stream, 22);

   // Scan lines must occupy multiples of four bytes
   int scanline_size = width * 3;
   int padding = 0;
   if (scanline_size % 4 != 0)
   {
      padding = 4 - scanline_size % 4;
   }

   if (file_size != start + (scanline_size + padding) * height)
   {
      cout << "Not a 24-bit true color image file." << endl;
      return 1;
   }
   
   int pos = start;

   for (int i = 0; i < height; i++) // For each scan line
   {
      for (int j = 0; j < width; j++) // For each pixel
      {
         
         stream.seekg(pos); // Go to the next pixel
         int blue = stream.get(); // Read the pixel
         int green = stream.get();
         int red = stream.get();

         bstream.seekg(pos);
         int reBlue = bstream.get(); // Read the pixel
         int reGreen = bstream.get();
         int reRed = bstream.get();


         process(blue, green, red, reBlue, reGreen, reRed); // Process the pixel

         if(((j>=130 && j<=200) && (i>=142.5 && i<=225)) || (j>200 && i < 100))
         {
         pstream.seekg(pos);
         int pblue = pstream.get(); // Read the pixel
         int pgreen = pstream.get();
         int pred = pstream.get();
         //blue = 55
         //green = 250
         //red = 37
         if(!((pblue >=-60 && pblue <= 160) && (pgreen >= 140 && pgreen <= 360) && (pred >= -80 && pred <= 140)))
         {
         ostream.seekp(pos); // Go back to the start of the pixel
         ostream.put(pblue); // Write the pixel
         ostream.put(pgreen);
         ostream.put(pred);
         }
            else
         {
         ostream.seekp(pos); // Go back to the start of the pixel
         ostream.put(blue); // Write the pixel
         ostream.put(green);
         ostream.put(red);
         }
         }
         
         else
         {
         ostream.seekp(pos); // Go back to the start of the pixel
         ostream.put(blue); // Write the pixel
         ostream.put(green);
         ostream.put(red);
         }
         pos = pos + 3;
      }

      stream.seekg(padding, ios::cur); // Skip the padding
      pos = pos + padding;
   }

   return 0;
}
