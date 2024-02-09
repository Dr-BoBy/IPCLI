#include "cimg.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

using Image = cimg_library::CImg<unsigned char>;

Image GrayScale(Image img){
    Image image = img;
    for(int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width(); ++x) {
            unsigned char r = image(x, y, 0); // canal rouge
            unsigned char g = image(x, y, 1); // canal vert
            unsigned char b = image(x, y, 2); // canal bleu

            unsigned char value = (r + g + b) / 3;

            // modification des 3 canaux
            image(x, y, 0) = value;
            image(x, y, 1) = value;
            image(x, y, 2) = value;
        }
    }
    return image;
}

Image ChanFactor(Image img, float rf, float gf, float bf){
    Image image = img;
    for(int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width(); ++x) {
            unsigned char r = image(x, y, 0); // canal rouge
            unsigned char g = image(x, y, 1); // canal vert
            unsigned char b = image(x, y, 2); // canal bleu

            // modification des 3 canaux
            image(x, y, 0) = r*rf;
            image(x, y, 1) = g*gf;
            image(x, y, 2) = b*bf;
        }
    }
    return image;
}

Image ChanSwap(Image img, char chanR, char chanG, char chanB){
    Image image = img;
    for(int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width(); ++x) {
            unsigned char r = image(x, y, 0); // canal rouge
            unsigned char g = image(x, y, 1); // canal vert
            unsigned char b = image(x, y, 2); // canal bleu

            // modification des 3 canaux
            switch (chanR){case 'g': image(x, y, 0) = g; break; case 'b': image(x, y, 0) = b; break; default: image(x, y, 0) = r; break;}
            switch (chanG){case 'r': image(x, y, 1) = r; break; case 'b': image(x, y, 1) = b; break; default: image(x, y, 1) = g; break;}
            switch (chanB){case 'r': image(x, y, 2) = r; break; case 'g': image(x, y, 2) = g; break; default: image(x, y, 3) = b; break;}
        }
    }
    return image;
}

Image Blur(Image img, int size){
    Image image = img;
    for(int y = size; y < image.height()-size; ++y) {
        for(int x = size; x < image.width()-size; ++x) {
            int valueR = 0;
            int valueG = 0;
            int valueB = 0;
            for(int i=-size; i<=size; i++){
                for(int j=-size; j<=size; j++){
                    valueR = valueR + image(x+j, y+i,0);
                    valueR = valueR + image(x+j, y+i,1);
                    valueR = valueR + image(x+j, y+i,2);
                }
            }
            image(x, y, 0) = valueR/(size*size);
            image(x, y, 1) = valueG/(size*size);
            image(x, y, 2) = valueB/(size*size);
        }
    }
    return image;
}

int main(int argc, char *argv[]) {

    //DEBUG
    if(argc == 2 && strcmp(argv[1], "help") == 0){
        std::cout << "\nUse :\n./IPCLI -i <InputFile> -o <OutputFile> <filter> \n\n";     
        std::cout << "Filter available : \n - grayscale \n - chanFactor <r> <g> <b>\n - chanSwap <chan> <chan> <chan>\n - blur <size>\n\n";
        return 0; 
    }
    else if(argc<5){
        std::cout << "Use at least ./IPCLI -i <InputFile> -o <OutputFile> <filter>\n";
        return 0;
    }

    //INPUT
    Image image(argv[2]); // charge le fichier input.jpgs



    //TRAITEMENT
    int argId = 5;
    while(argId<argc){
        if(strcmp(argv[argId],"grayscale")==0){
            std::cout << "Applaying GrayScale filter...\n";
            image = GrayScale(image);
            argId=argId+1;
        }
        else if(strcmp(argv[argId],"chanFactor")==0){
            std::cout << "Applaying Channel Factor filter...\n";
            image = ChanFactor(image, atof(argv[argId+1]),atof(argv[argId+2]),atof(argv[argId+3]));
            argId=argId+4;
        }
        else if(strcmp(argv[argId],"chanSwap")==0){
            std::cout << "Applaying Channel Swap filter...\n";
            image = ChanSwap(image, *argv[argId+1], *argv[argId+2], *argv[argId+3]);
            argId=argId+4;
        }
        else if(strcmp(argv[argId],"blur")==0){
            std::cout << "Applaying Blur filter...\n";
            image = Blur(image, strtol(argv[argId+1],NULL, 10));
            argId=argId+2;
        }
        else{
            std::cout << "Argument error => " << argv[argId] << "\nUse : ./IPCLI help\n";
            break;
        }
    }
    std::cout << "Done\n";
    
    //OUTPUT
    image.save(argv[4]); // enregistre l'image modifiée
}

