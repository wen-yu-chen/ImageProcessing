#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Image {
    struct Header {
        char idLength;
        char colorMapType;
        char dataTypeCode;
        short colorMapOrigin;
        short colorMapLength;
        char colorMapDepth;
        short xOrigin;
        short yOrigin;
        short width;
        short height;
        char bitsPerPixel;
        char imageDescriptor;
    };

    struct Pixel {
        unsigned char blue;
        unsigned char green;
        unsigned char red;
    };

    Header header;
    vector< vector<Pixel> > color;
};


void ReadFile(string s, Image &iobj) {
    ifstream f;
    f.open(s, ios::binary);

    f.read((char*)&iobj.header.idLength, sizeof(char));
    f.read((char*)&iobj.header.colorMapType, sizeof(char));
    f.read((char*)&iobj.header.dataTypeCode, sizeof(char));
    f.read((char*)&iobj.header.colorMapOrigin, sizeof(short));
    f.read((char*)&iobj.header.colorMapLength, sizeof(short));
    f.read((char*)&iobj.header.colorMapDepth, sizeof(char));
    f.read((char*)&iobj.header.xOrigin, sizeof(short));
    f.read((char*)&iobj.header.yOrigin, sizeof(short));
    f.read((char*)&iobj.header.width, sizeof(short));
    f.read((char*)&iobj.header.height, sizeof(short));
    f.read((char*)&iobj.header.bitsPerPixel, sizeof(char));
    f.read((char*)&iobj.header.imageDescriptor, sizeof(char));

    for (unsigned int i = 0; i < iobj.header.height; i++) {
        Image::Pixel temp;
        iobj.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj.header.width; j++) {
            f.read((char*)&temp.blue, sizeof(char));
            f.read((char*)&temp.green, sizeof(char));
            f.read((char*)&temp.red, sizeof(char));

            iobj.color[i].push_back(temp);
        }
    }

    f.close();
}


void WriteFile (string s, Image &iobj) {
    ofstream o (s, ios_base::binary);

    o.write((char*)&iobj.header.idLength, sizeof(char));
    o.write((char*)&iobj.header.colorMapType, sizeof(char));
    o.write((char*)&iobj.header.dataTypeCode, sizeof(char));
    o.write((char*)&iobj.header.colorMapOrigin, sizeof(short));
    o.write((char*)&iobj.header.colorMapLength, sizeof(short));
    o.write((char*)&iobj.header.colorMapDepth, sizeof(char));
    o.write((char*)&iobj.header.xOrigin, sizeof(short));
    o.write((char*)&iobj.header.yOrigin, sizeof(short));

    o.write((char*)&iobj.header.width, sizeof(short));
    o.write((char*)&iobj.header.height, sizeof(short));
    o.write((char*)&iobj.header.bitsPerPixel, sizeof(char));
    o.write((char*)&iobj.header.imageDescriptor, sizeof(char));

    for (unsigned int i = 0; i < iobj.header.height; i++) {

        for (unsigned int j = 0; j < iobj.header.width; j++) {
            o.write((char*)&iobj.color[i][j].blue, sizeof(char));
            o.write((char*)&iobj.color[i][j].green, sizeof(char));
            o.write((char*)&iobj.color[i][j].red, sizeof(char));
        }
    }

    o.close();
}


void MultiplyMode (Image &iobj1, Image &iobj2, Image &iobj3) {
    Image::Pixel temp;
    iobj3.header = iobj2.header;

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {
        iobj3.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj1.color.size(); j++) {
            temp.blue = (unsigned char)(((((iobj1.color[i][j].blue) / 255.0f) * ((iobj2.color[i][j].blue) / 255.0f)) * 255.0f) + 0.5f);
            temp.green = (unsigned char)(((((iobj1.color[i][j].green) / 255.0f) * ((iobj2.color[i][j].green) / 255.0f)) * 255.0f) + 0.5f);
            temp.red = (unsigned char)(((((iobj1.color[i][j].red) / 255.0f) * ((iobj2.color[i][j].red) / 255.0f)) * 255.0f) + 0.5f);

            iobj3.color[i].push_back(temp);
        }
    }
}


void SubtractMode (Image &iobj1, Image &iobj2, Image &iobj3) {
    Image::Pixel temp;
    iobj3.header = iobj2.header;

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {
        iobj3.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj1.color.size(); j++) {

            if (iobj1.color[i][j].blue < iobj2.color[i][j].blue)
            temp.blue = (unsigned char)(0.0f);

            else {
                temp.blue = iobj1.color[i][j].blue - iobj2.color[i][j].blue;
            }

            if (iobj1.color[i][j].green < iobj2.color[i][j].green)
                temp.green = (unsigned char)(0.0f);

            else {
                temp.green = iobj1.color[i][j].green - iobj2.color[i][j].green;
            }

            if (iobj1.color[i][j].red < iobj2.color[i][j].red)
                temp.red = (unsigned char)(0.0f);

            else {
                temp.red = iobj1.color[i][j].red - iobj2.color[i][j].red;
            }

            iobj3.color[i].push_back(temp);
        }
    }
}


void ScreenMode (Image &iobj1, Image &iobj2, Image &iobj3) {
    Image::Pixel temp;
    iobj3.header = iobj2.header;

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {
        iobj3.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj1.color.size(); j++) {
            temp.blue = (unsigned char) (((1 - (1 - (iobj1.color[i][j].blue / 255.0f)) * (1 - (iobj2.color[i][j].blue / 255.0f))) * 255.0f) + 0.5f);
            temp.green = (unsigned char) (((1 - (1 - (iobj1.color[i][j].green / 255.0f)) * (1 - (iobj2.color[i][j].green / 255.0f))) * 255.0f) + 0.5f);
            temp.red = (unsigned char) (((1 - (1 - (iobj1.color[i][j].red / 255.0f)) * (1 - (iobj2.color[i][j].red / 255.0f))) * 255.0f) + 0.5f);

            iobj3.color[i].push_back(temp);
        }
    }
}


void OverlayMode (Image &iobj1, Image &iobj2, Image &iobj3) {
    Image::Pixel temp;
    iobj3.header = iobj2.header;

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {
        iobj3.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj1.color.size(); j++) {

            if ((iobj2.color[i][j].blue / 255.0f) <= 0.5f) {
                temp.blue = (unsigned char) (((2 * (iobj1.color[i][j].blue / 255.0f) * (iobj2.color[i][j].blue / 255.0f)) * 255.0f) + 0.5f);
            }

            else {
                temp.blue = (unsigned char) (((1 - (2 * (1 - (iobj1.color[i][j].blue / 255.0f)) * (1 - (iobj2.color[i][j].blue / 255.0f)))) * 255.0f) + 0.5f);
            }

            if ((iobj2.color[i][j].green / 255.0f) <= 0.5f) {
                temp.green = (unsigned char) (((2 * (iobj1.color[i][j].green / 255.0f) * (iobj2.color[i][j].green / 255.0f)) * 255.0f) + 0.5f);
            }

            else {
                temp.green = (unsigned char) (((1 - (2 * (1 - (iobj1.color[i][j].green / 255.0f)) * (1 - (iobj2.color[i][j].green / 255.0f)))) * 255.0f) + 0.5f);
            }

            if ((iobj2.color[i][j].red / 255.0f) <= 0.5f) {
                temp.red = (unsigned char) (((2 * (iobj1.color[i][j].red / 255.0f) * (iobj2.color[i][j].red / 255.0f)) * 255.0f) + 0.5f);
            }

            else {
                temp.red = (unsigned char) (((1 - (2 * (1 - (iobj1.color[i][j].red / 255.0f)) * (1 - (iobj2.color[i][j].red / 255.0f)))) * 255.0f) + 0.5f);
            }

            iobj3.color[i].push_back(temp);
        }
    }
}


void AddGreen (Image &iobj1, Image &iobj3) {
    Image::Pixel temp;
    iobj3.header = iobj1.header;

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {
        iobj3.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj1.color.size(); j++) {
            temp.blue = iobj1.color[i][j].blue;

            if (iobj1.color[i][j].green >= 55.0f) {
                temp.green = (unsigned char) (255.0f);

            }

            else {
                temp.green = (unsigned char) ((iobj1.color[i][j].green + 200.0f) + 0.5f);
            }

            temp.red = iobj1.color[i][j].red;

            iobj3.color[i].push_back(temp);
        }
    }
}


void Scale (Image &iobj1, Image &iobj3) {
    Image::Pixel temp;
    iobj3.header = iobj1.header;

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {
        iobj3.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj1.color.size(); j++) {
            temp.blue = (unsigned char) (0.0f);
            temp.green = iobj1.color[i][j].green;
            if (iobj1.color[i][j].red > 63) {
                temp.red = (unsigned char) (255.0f);
            }
            else {
                temp.red = (unsigned char) (((iobj1.color[i][j].red) * 4.0f) + 0.5f);
            }

            iobj3.color[i].push_back(temp);
        }
    }
}


void Separate (Image &iobj1, Image &iobj3, unsigned int x) {
    Image::Pixel temp;
    iobj3.header = iobj1.header;

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {
        iobj3.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj1.color.size(); j++) {

            if (x == 1) {
                temp.blue = iobj1.color[i][j].blue;
                temp.green = iobj1.color[i][j].blue;
                temp.red = iobj1.color[i][j].blue;
            }

            else if (x == 2) {
                temp.blue = iobj1.color[i][j].green;
                temp.green = iobj1.color[i][j].green;
                temp.red = iobj1.color[i][j].green;
            }

            else if (x == 3) {
                temp.blue = iobj1.color[i][j].red;
                temp.green = iobj1.color[i][j].red;
                temp.red = iobj1.color[i][j].red;
            }

            else {
                cout << "Not valid!" << endl;

                break;
            }

            iobj3.color[i].push_back(temp);
        }
    }
}


void Combine (Image &iobj1, Image &iobj2, Image &iobj3, Image &iobj4) {
    Image::Pixel temp;
    iobj4.header = iobj1.header;

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {
        iobj4.color.push_back(vector<Image::Pixel>());

        for (unsigned int j = 0; j < iobj1.color[0].size(); j++) {
            temp.blue = iobj1.color[i][j].blue;
            temp.green = iobj2.color[i][j].green;
            temp.red = iobj3.color[i][j].red;

            iobj4.color[i].push_back(temp);
        }
    }
}


void Rotate (Image &iobj) {
    Image::Pixel temp;

    for (unsigned int i = 0; i < (iobj.color.size()) / 2; i++) {

        for (unsigned int j = 0; j < iobj.color[0].size(); j++) {
            temp.blue = iobj.color[i][j].blue;
            iobj.color[i][j].blue = iobj.color[iobj.color.size() - 1 - i][iobj.color[0].size() - 1 -j].blue;
            iobj.color[iobj.color.size() - 1 - i][iobj.color[0].size() - 1 -j].blue = temp.blue;

            temp.green = iobj.color[i][j].green;
            iobj.color[i][j].green = iobj.color[iobj.color.size() - 1 - i][iobj.color[0].size() - 1 -j].green;
            iobj.color[iobj.color.size() - 1 - i][iobj.color[0].size() - 1 -j].green = temp.green;

            temp.red = iobj.color[i][j].red;
            iobj.color[i][j].red = iobj.color[iobj.color.size() - 1 - i][iobj.color[0].size() - 1 -j].red;
            iobj.color[iobj.color.size() - 1 - i][iobj.color[0].size() - 1 -j].red = temp.red;
        }
    }
}


void WriteExtraCredit(string s, Image &extra, Image &car, Image &circles, Image &pattern1, Image &text) {
    ofstream o (s, ios_base::binary);

    o.write((char*)&extra.header.idLength, sizeof(char));
    o.write((char*)&extra.header.colorMapType, sizeof(char));
    o.write((char*)&extra.header.dataTypeCode, sizeof(char));
    o.write((char*)&extra.header.colorMapOrigin, sizeof(short));
    o.write((char*)&extra.header.colorMapLength, sizeof(short));
    o.write((char*)&extra.header.colorMapDepth, sizeof(char));
    o.write((char*)&extra.header.xOrigin, sizeof(short));
    o.write((char*)&extra.header.yOrigin, sizeof(short));

    o.write((char*)&extra.header.width, sizeof(short));
    o.write((char*)&extra.header.height, sizeof(short));
    o.write((char*)&extra.header.bitsPerPixel, sizeof(char));
    o.write((char*)&extra.header.imageDescriptor, sizeof(char));

    //lower part of image
    for (unsigned int i = 0; i < 512; i++) {

        for (unsigned int j = 0; j < 1024; j++) {

            if (j < 512) {
                o.write((char *) &text.color[i][j].blue, sizeof(char));
                o.write((char *) &text.color[i][j].green, sizeof(char));
                o.write((char *) &text.color[i][j].red, sizeof(char));
            }

            else {
                o.write((char *) &pattern1.color[i][j - 512].blue, sizeof(char));
                o.write((char *) &pattern1.color[i][j - 512].green, sizeof(char));
                o.write((char *) &pattern1.color[i][j - 512].red, sizeof(char));
            }
        }
    }

    //upper part of image
    for (unsigned int i = 512; i < 1024; i++) {

        for (unsigned int j = 0; j < 1024; j++) {

            if (j < 512) {
                o.write((char *) &car.color[i - 512][j].blue, sizeof(char));
                o.write((char *) &car.color[i - 512][j].green, sizeof(char));
                o.write((char *) &car.color[i - 512][j].red, sizeof(char));
            }
            else {
                o.write((char *) &circles.color[i - 512][j - 512].blue, sizeof(char));
                o.write((char *) &circles.color[i - 512][j - 512].green, sizeof(char));
                o.write((char *) &circles.color[i - 512][j - 512].red, sizeof(char));
            }
        }
    }

    o.close();
}


void Test(Image &iobj1, Image &iobj2) {
    bool passed = true;

    if (iobj1.header.idLength != iobj2.header.idLength) {
        passed = false;
    }

    else if (iobj1.header.colorMapType != iobj2.header.colorMapType) {
        passed = false;
    }

    else if (iobj1.header.dataTypeCode != iobj2.header.dataTypeCode) {
        passed = false;
    }

    else if (iobj1.header.colorMapType != iobj2.header.colorMapType) {
        passed = false;
    }

    else if (iobj1.header.colorMapOrigin != iobj2.header.colorMapOrigin) {
        passed = false;
    }

    else if (iobj1.header.colorMapLength != iobj2.header.colorMapLength) {
        passed = false;
    }

    else if (iobj1.header.colorMapDepth != iobj2.header.colorMapDepth) {
        passed = false;
    }

    else if (iobj1.header.xOrigin != iobj2.header.xOrigin) {
        passed = false;
    }

    else if (iobj1.header.yOrigin != iobj2.header.yOrigin) {
        passed = false;
    }

    else if (iobj1.header.width != iobj2.header.width) {
        passed = false;
    }

    else if (iobj1.header.height != iobj2.header.height) {
        passed = false;
    }

    else if (iobj1.header.bitsPerPixel != iobj2.header.bitsPerPixel) {
        passed = false;
    }

    else if (iobj1.header.imageDescriptor != iobj2.header.imageDescriptor) {
        passed = false;
    }

    for (unsigned int i = 0; i < iobj1.color.size(); i++) {

        for (unsigned int j = 0; j < iobj1.color[0].size(); j++) {

            if (iobj1.color[i][j].blue != iobj2.color[i][j].blue) {
                passed = false;
            }

            else if (iobj1.color[i][j].green != iobj2.color[i][j].green) {
                passed = false;
            }

            else if (iobj1.color[i][j].red != iobj2.color[i][j].red) {
                passed = false;
            }
        }
    }

    if (!passed) {
        cout << "Failed!" << endl;
    }

    else {
        cout << "Passed!" << endl;
    }
}


int main() {
    cout << "Task 1: " << endl;
    Image newImage1;
    Image task1A;
    Image task1B;
    ReadFile("input/layer1.tga", task1A);
    ReadFile("input/pattern1.tga", task1B);
    MultiplyMode(task1A, task1B, newImage1);
    WriteFile("output/part1.tga", newImage1);

    cout << "Task 2: " << endl;
    Image newImage2;
    Image task2A;
    Image task2B;
    ReadFile("input/layer2.tga", task2A);
    ReadFile("input/car.tga", task2B);
    SubtractMode(task2B, task2A, newImage2);
    WriteFile("output/part2.tga", newImage2);

    cout << "Task 3: " << endl;
    Image newImage3;
    Image task3A;
    Image task3B;
    Image task3temp1;
    ReadFile("input/layer1.tga", task3A);
    ReadFile("input/pattern2.tga", task3B);
    MultiplyMode(task3A, task3B, task3temp1);
    Image task3C;
    ReadFile("input/text.tga", task3C);
    ScreenMode(task3temp1, task3C, newImage3);
    WriteFile("output/part3.tga", newImage3);

    cout << "Task 4: " << endl;
    Image newImage4;
    Image task4A;
    Image task4B;
    Image task4temp1;
    ReadFile("input/layer2.tga", task4A);
    ReadFile("input/circles.tga", task4B);
    MultiplyMode(task4A, task4B, task4temp1);
    Image task4C;
    ReadFile("input/pattern2.tga", task4C);
    SubtractMode(task4temp1, task4C, newImage4);
    WriteFile("output/part4.tga", newImage4);

    cout << "Task 5: " << endl;
    Image newImage5;
    Image task5A;
    Image task5B;
    ReadFile("input/layer1.tga", task5A);
    ReadFile("input/pattern1.tga", task5B);
    OverlayMode(task5A, task5B, newImage5);
    WriteFile("output/part5.tga", newImage5);

    cout << "Task 6: " << endl;
    Image newImage6;
    Image task6;
    ReadFile("input/car.tga", task6);
    AddGreen(task6, newImage6);
    WriteFile("output/part6.tga", newImage6);

    cout << "Task 7: " << endl;
    Image newImage7;
    Image task7;
    ReadFile("input/car.tga", task7);
    Scale(task7, newImage7);
    WriteFile("output/part7.tga", newImage7);

    cout << "Task 8: " << endl;
    Image newImage8_B;
    Image newImage8_G;
    Image newImage8_R;
    Image task8;
    ReadFile("input/car.tga", task8);
    Separate(task8, newImage8_B, 1);
    Separate(task8, newImage8_G, 2);
    Separate(task8, newImage8_R, 3);
    WriteFile("output/part8_b.tga", newImage8_B);
    WriteFile("output/part8_g.tga", newImage8_G);
    WriteFile("output/part8_r.tga", newImage8_R);

    cout << "Task 9: " << endl;
    Image newImage9;
    Image task9_B;
    Image task9_G;
    Image task9_R;
    ReadFile("input/layer_blue.tga", task9_B);
    ReadFile("input/layer_green.tga", task9_G);
    ReadFile("input/layer_red.tga", task9_R);
    Combine(task9_B, task9_G, task9_R, newImage9);
    WriteFile("output/part9.tga", newImage9);

    cout << "Task 10: " << endl;
    Image newImage10;
    ReadFile("input/text2.tga", newImage10);
    Rotate(newImage10);
    WriteFile("output/part10.tga", newImage10);

    cout << "Extra Credit: \n" << endl;
    Image extraImage;
    Image car;
    Image circles;
    Image pattern1;
    Image text;
    ReadFile("input/car.tga", car);
    ReadFile("input/circles.tga", circles);
    ReadFile("input/pattern1.tga", pattern1);
    ReadFile("input/text.tga", text);
    extraImage.header = car.header;
    extraImage.header.width = 1024;
    extraImage.header.height = 1024;
    WriteExtraCredit("output/extracredit.tga", extraImage, car, circles, pattern1, text);


    //Test begins
    cout << "\n";
    cout << "Test Starts Here\n" << endl;

    //Test1
    Image testImage1;
    ReadFile("examples/EXAMPLE_part1.tga", testImage1);
    cout << "Test #1             ";
    Test(newImage1, testImage1);

    //Test2
    Image testImage2;
    ReadFile("examples/EXAMPLE_part2.tga", testImage2);
    cout << "Test #2             ";
    Test(newImage2, testImage2);

    //Test3
    Image testImage3;
    ReadFile("examples/EXAMPLE_part3.tga", testImage3);
    cout << "Test #3             ";
    Test(newImage3, testImage3);

    //Test4
    Image testImage4;
    ReadFile("examples/EXAMPLE_part4.tga", testImage4);
    cout << "Test #4             ";
    Test(newImage4, testImage4);

    //Test5
    Image testImage5;
    ReadFile("examples/EXAMPLE_part5.tga", testImage5);
    cout << "Test #5             ";
    Test(newImage5, testImage5);

    //Test6
    Image testImage6;
    ReadFile("examples/EXAMPLE_part6.tga", testImage6);
    cout << "Test #6             ";
    Test(newImage6, testImage6);

    //Test7
    Image testImage7;
    ReadFile("examples/EXAMPLE_part7.tga", testImage7);
    cout << "Test #7             ";
    Test(newImage7, testImage7);

    //Test8
    Image testImage8_B;
    Image testImage8_G;
    Image testImage8_R;
    ReadFile("examples/EXAMPLE_part8_b.tga", testImage8_B);
    ReadFile("examples/EXAMPLE_part8_g.tga", testImage8_G);
    ReadFile("examples/EXAMPLE_part8_r.tga", testImage8_R);
    cout << "Test #8 Blue        ";
    Test(newImage8_B, testImage8_B);
    cout << "Test    Green       ";
    Test(newImage8_G, testImage8_G);
    cout << "Test    Red         ";
    Test(newImage8_R, testImage8_R);

    //Test9
    Image testImage9;
    ReadFile("examples/EXAMPLE_part9.tga", testImage9);
    cout << "Test #9             ";
    Test(newImage9, testImage9);

    //Test10
    Image testImage10;
    ReadFile("examples/EXAMPLE_part10.tga", testImage10);
    cout << "Test #10            ";
    Test(newImage10, testImage10);

    //Text Extra Credit
    Image testExtra;
    Image myExtra;
    ReadFile("examples/EXAMPLE_extraCredit.tga", testExtra);
    ReadFile("output/extracredit.tga", myExtra);
    cout << "Test Extra Credit   ";
    Test(myExtra, testExtra);


    return 0;
}