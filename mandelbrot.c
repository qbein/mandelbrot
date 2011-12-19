#import <stdlib.h>
#import <stdio.h>
#import <math.h>

#define MIN_X           -2.25
#define MAX_X           1.0
#define MIN_Y           -1.25
#define MAX_Y           1.25

#define COLORS          250
#define START_COLOR     {   3,  18,  33 }
#define MIDDLE_COLOR    { 255, 255, 255 }
#define END_COLOR       { 144,  82,  13 }

typedef struct {
  int screenX;
  int screenY;
  int iterations;
} Config;

typedef struct {
    float red;
    float green;
    float blue;
} Color;


void usage(char filename[]) {
    printf("usage: %s x y iterations", filename);
}

void palette_from_to(Color palette[], Color colstart, Color colend, int start, int max) {
    int cnt = max - start;
    float rfact = (float)(colend.red - colstart.red) / cnt;
    float gfact = (float)(colend.green - colstart.green) / cnt;
    float bfact = (float)(colend.blue - colstart.blue) / cnt;
    
    int i;
    for(i = 1; i<=cnt; i++) {
        float red = (float)colstart.red + rfact * (float)i;
        float green = (float)colstart.green + gfact * (float)i;
        float blue = (float)colstart.blue + bfact * (float)i;
        Color current = { red, green, blue };
        palette[start+i-1] = current;
    }
}

void palette_generate(Color palette[]) {
    Color start = START_COLOR;
    Color middle = MIDDLE_COLOR;
    Color end = END_COLOR;
    
    palette_from_to(palette, start, middle, 0, COLORS / 4);
    palette_from_to(palette, middle, end, COLORS / 4, COLORS);
}

void init_ppm(Config conf) {
    // Line 1: P3 means colors are in ASCII
    // Line 2: Resolution for X Y,
    // Line 3: Colors 255 per channel
    printf("P3\n");
    printf("%d %d\n", conf.screenX, conf.screenY);
    printf("255\n");
}

void plot(int x, int y, Color c) {
    printf("%03d %03d %03d ", (int)(c.red), (int)(c.green), (int)(c.blue));
}


int main(int argc, char *argv[]) {
    int sy, sx;
    
    if(argc != 4) {
      usage(argv[0]);
      return 1;
    }

    Config conf = { atoi(argv[1]), atoi(argv[2]), atoi(argv[3]) };

    if(conf.screenX == 0 || conf.screenY == 0 || conf.iterations == 0) {
      usage(argv[0]);
      return 1;
    }

    Color palette[COLORS];
    palette_generate(palette);
    init_ppm(conf);
   
    float xrange = MAX_X - MIN_X;
    float yrange = MAX_Y - MIN_Y;
    
    for(sy = 1; sy <= conf.screenY; sy++) {
        for(sx = 1; sx <= conf.screenX; sx++) {
            float x, y;
            x = 0.0;
            y = 0.0;
            
            float x0 = sx / (float)conf.screenX * xrange + MIN_X; 
            float y0 = sy / (float)conf.screenY * yrange + MIN_Y; 
        
            int i = 0;
            while(((x*x - y*y) < (2*2)) && i < conf.iterations) {
                float xtmp;
                xtmp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtmp;
                i++;
            }
            
            Color c = { 0, 0, 0 };
            if((x*x + y*y) > (2*2)) {
                c = palette[i % COLORS];
            }
            plot(sx, sy, c);
        }

        printf("\n");
    }
    
    return 0;
}
