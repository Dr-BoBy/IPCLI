#include "cimg.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <thread>

using Image = cimg_library::CImg<unsigned char>;

//Image img(100, 50, 1, 3, 0);

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
            switch (chanR){case 'g': image(x, y, 0) = g; break; case 'b': image(x, y, 0) = b; break;}
            switch (chanG){case 'r': image(x, y, 1) = r; break; case 'b': image(x, y, 1) = b; break;}
            switch (chanB){case 'r': image(x, y, 2) = r; break; case 'g': image(x, y, 2) = g; break;}
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
                    valueR = valueR + (int) img(x+j, y+i,0);
                    valueG = valueG + (int) img(x+j, y+i,1);
                    valueB = valueB + (int) img(x+j, y+i,2) ;
                }
            }
            image(x, y, 0) = valueR/((size*2+1)*(size*2+1));
            image(x, y, 1) = valueG/((size*2+1)*(size*2+1));
            image(x, y, 2) = valueB/((size*2+1)*(size*2+1));
        }
    }
    return image;
}

Image Extract(Image img, char chan){
    Image image = img;
    for(int y = 0; y < image.height(); ++y) {
        for(int x = 0; x < image.width(); ++x) {
            unsigned char r = image(x, y, 0); // canal rouge
            unsigned char g = image(x, y, 1); // canal vert
            unsigned char b = image(x, y, 2); // canal bleu

            // modification des 3 canaux
            image(x, y, 0) = 0;
            image(x, y, 1) = 0;
            image(x, y, 2) = 0;
            switch (chan){case 'r': image(x, y, 0) = r; break; case 'g': image(x, y, 1) = g; break; case 'b': image(x, y, 2) = b; break;}

        }
    }
    return image;
}

Image Pixel(Image img, int size){
    Image image = img;
    for(int y = size; y < image.height()-size; y=y+size*2+1) {
        for(int x = size; x < image.width()-size; x=x+size*2+1) {
            int valueR = 0;
            int valueG = 0;
            int valueB = 0;
            for(int i=-size; i<=size; i++){
                for(int j=-size; j<=size; j++){
                    valueR = valueR + (int) image(x+j, y+i,0);
                    valueG = valueG + (int) image(x+j, y+i,1);
                    valueB = valueB + (int) image(x+j, y+i,2) ;
                }
            }
            for(int i=-size; i<=size; i++){
                for(int j=-size; j<=size; j++){
                    image(x+j, y+i, 0) = valueR/((size*2+1)*(size*2+1));
                    image(x+j, y+i, 1) = valueG/((size*2+1)*(size*2+1));
                    image(x+j, y+i, 2) = valueB/((size*2+1)*(size*2+1));
                }
            }
        }
    }
    return image;
}

int main(int argc, char *argv[]) {

    //DEBUG
    if(argc == 2 && strcmp(argv[1], "help") == 0){
        std::cout << "\nUse :\n./IPCLI -i <InputFile> -o <OutputFile> <filter> \n\n";     
        std::cout << "Filter available : \n - grayscale \n - chanFactor <r> <g> <b>\n - chanSwap <chan> <chan> <chan>\n - blur <size>\n - extract <chan>\n - pixel <pixelFactor> <autoFit>\n\n";
        return 0; 
    }
    else if(argc<5){
        std::cout << "Use at least ./IPCLI -i <InputFile> -o <OutputFile> <filter>\n";
        return 0;
    }

    //INPUT
    Image image(argv[2]); // charge le fichier input.jpg;
    



    //TRAITEMENT
    int argId = 5;
    while(argId<argc){
        if(strcmp(argv[argId],"grayscale")==0){
            std::cout << "Applaying GrayScale filter...\n";
            std::thread worker([&image]{image = GrayScale(image);});
            worker.join();
            argId=argId+1;
        }
        else if(strcmp(argv[argId],"chanFactor")==0){
            std::cout << "Applaying Channel Factor filter...\n";
            std::thread worker([&image, &argv, &argId]{image = ChanFactor(image, atof(argv[argId+1]),atof(argv[argId+2]),atof(argv[argId+3]));});
            worker.join();
            argId=argId+4;
        }
        else if(strcmp(argv[argId],"chanSwap")==0){
            if(*argv[argId+1] != 'r' && *argv[argId+1] != 'g' && *argv[argId+1] != 'b'){
                std::cout << "Unknown channel => " << argv[argId+1] << "\nAvailable channels are r/g/b\n";
                break;
            }
            else if(*argv[argId+2] != 'r' && *argv[argId+2] != 'g' && *argv[argId+2] != 'b'){
                std::cout << "Unknown channel => " << argv[argId+2] << "\nAvailable channels are r/g/b\n";
                break;
            }
            else if(*argv[argId+3] != 'r' && *argv[argId+3] != 'g' && *argv[argId+3] != 'b'){
                std::cout << "Unknown channel => " << argv[argId+3] << "\nAvailable channels are r/g/b\n";
                break;
            }
            std::cout << "Applaying Channel Swap filter...\n";
            std::thread worker([&image, &argv, &argId]{image = ChanSwap(image, *argv[argId+1], *argv[argId+2], *argv[argId+3]);});
            worker.join();
            argId=argId+4;
        }
        else if(strcmp(argv[argId],"blur")==0){
            std::cout << "Applaying Blur filter...\n";
            std::thread worker([&image, &argv, &argId]{image = Blur(image, strtol(argv[argId+1],NULL, 10));});
            worker.join();
            argId=argId+2;
        }
        else if(strcmp(argv[argId],"extract")==0){
            if(*argv[argId+1] != 'r' && *argv[argId+1] != 'g' && *argv[argId+1] != 'b'){
                std::cout << "Unknown channel => " << argv[argId+1] << "\nAvailable channels are r/g/b\n";
                break;
            }
            std::cout << "Extracting channel...\n";
            std::thread worker([&image, &argv, &argId]{image = Extract(image, *argv[argId+1]);});
            worker.join();
            argId=argId+2;
        }
        else if(strcmp(argv[argId],"pixel")==0){
            int size = strtol(argv[argId+1],NULL, 10);
            if(strcmp(argv[argId+2],"true")==0){
                int max = size;
                int i=size;
                while(i>1){
                    std::cout << i << "\n";
                    if(image.height()%(2*i+1) ==0 && image.width()%(2*i+1)==0){
                        max = i;
                        break;
                    }
                    i--;
                }
                if(max==size){
                    std::cout << "/!\\ Warning : Unable to autofit !\n";     
                }else{
                    std::cout << "Autofit enable -> New pixel factor : "<< max <<"\n";
                    size = max;  
                }
            }else{
                if(image.height()%(size*2+1) != 0 || image.width()%(size*2+1) != 0) {
                    std::cout << "/!\\ Warning : Pixel factor not fit perfectly image !\n";
                }
            } 
            std::cout << "Applaying pixel filter...\n";
            std::thread worker([&image, &size]{image = Pixel(image, size);});
            worker.join();
            argId=argId+3;
        }
        else{
            std::cout << "Argument error => " << argv[argId] << "\nUse : ./IPCLI help\n";
            break;
        }
    }
    if(argId>=argc){
        std::cout << "Done\n";
        //OUTPUT
        image.save(argv[4]); // enregistre l'image modifiée
    }
   
    
    
}

