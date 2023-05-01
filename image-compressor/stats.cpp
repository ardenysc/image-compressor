
#include "stats.h"

// Initialize the private vectors so that, for each color, entry
// (x,y) is the cumulative sum of the the color values in the rectangle from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares in the rectangle from (0,0) to (x,y).
stats::stats(PNG& im) {
    /* Your code here!! */
    int r = 0;
    int g = 0;
    int b = 0;
    int sq_r = 0;
    int sq_b = 0;
    int sq_g = 0;

    sumRed.resize(im.width());
    sumGreen.resize(im.width());
    sumBlue.resize(im.width());
    sumsqRed.resize(im.width());
    sumsqGreen.resize(im.width());
    sumsqBlue.resize(im.width());

    for (unsigned int x = 0; x < im.width(); x++) {
        sumRed[x].resize(im.height());
        sumGreen[x].resize(im.height());
        sumBlue[x].resize(im.height());
        sumsqRed[x].resize(im.height());
        sumsqGreen[x].resize(im.height());
        sumsqBlue[x].resize(im.height());
        for (unsigned int y = 0; y < im.height(); y++) {
            RGBAPixel *pxl = im.getPixel(x, y);
            r += pxl->r;
            g += pxl->g;
            b += pxl->b;
            sq_r += pow(pxl->r, 2);
            sq_g += pow(pxl->g, 2);
            sq_b += pow(pxl->b, 2);
            sumRed[x][y] = r;
            sumGreen[x][y] = g;
            sumBlue[x][y] = b;
            sumsqRed[x][y] = sq_r;
            sumsqGreen[x][y] = sq_g;
            sumsqBlue[x][y] = sq_b;
        }
    }
}

/** Returns the sum of all pixel values in the given colour channel 
 * in the square defined by ul and dim
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the square
* @param dim is log of side length of the square */
long stats::getSum(char channel, pair<int, int> ul, int dim) {
    /* Your code here!! */
    long r_sum = 0;
    long g_sum = 0;
    long b_sum = 0;
    int length = pow(2, dim);
    unsigned int lr_x = ul.first + length - 1;
    unsigned int lr_y = ul.second + length - 1;
    unsigned int y = ul.second;
    
    for (unsigned int x = ul.first; x <= lr_x; x++) {
        if (x == 0 && y == 0) {
            r_sum += sumRed[x][lr_y];
            g_sum += sumGreen[x][lr_y];
            b_sum += sumBlue[x][lr_y];
        } else if (y == 0) {
            r_sum += sumRed[x][lr_y] - sumRed[x - 1][sumRed[0].size() - 1];
            g_sum += sumGreen[x][lr_y] - sumGreen[x - 1][sumGreen[0].size() - 1];
            b_sum += sumBlue[x][lr_y] - sumBlue[x - 1][sumBlue[0].size() - 1];
        } else {
            r_sum += sumRed[x][lr_y] - sumRed[x][y - 1];
            g_sum += sumGreen[x][lr_y] - sumGreen[x][y - 1];
            b_sum += sumBlue[x][lr_y] - sumBlue[x][y - 1];
        }
    }

    switch (channel)
    {
    case 'r':
        return r_sum;
        break;
    case 'g':
        return g_sum;
        break;
    case 'b':
        return b_sum;
        break;
    default:
        break;
    }
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    /* Your code here!! */
    long r_sum = 0;
    long g_sum = 0;
    long b_sum = 0;
    int length = pow(2, dim);
    unsigned int lr_x = ul.first + length - 1;
    unsigned int lr_y = ul.second + length - 1;
    unsigned int y = ul.second;
    
    for (unsigned int x = ul.first; x <= lr_x; x++) {
        if (x == 0 && y == 0) {
            r_sum += sumsqRed[x][lr_y];
            g_sum += sumsqGreen[x][lr_y];
            b_sum += sumsqBlue[x][lr_y];
        } else if (y == 0) {
            r_sum += sumsqRed[x][lr_y] - sumsqRed[x - 1][sumsqRed[0].size() - 1];
            g_sum += sumsqGreen[x][lr_y] - sumsqGreen[x - 1][sumsqGreen[0].size() - 1];
            b_sum += sumsqBlue[x][lr_y] - sumsqBlue[x - 1][sumsqBlue[0].size() - 1];
        } else {
            r_sum += sumsqRed[x][lr_y] - sumsqRed[x][y - 1];
            g_sum += sumsqGreen[x][lr_y] - sumsqGreen[x][y - 1];
            b_sum += sumsqBlue[x][lr_y] - sumsqBlue[x][y - 1];
        }
    }

    switch (channel)
    {
    case 'r':
        return r_sum;
        break;
    case 'g':
        return g_sum;
        break;
    case 'b':
        return b_sum;
        break;
    default:
        break;
    }
}

/** Given a square, return the number of pixels in the square
* @param dim is log of side length of the square */
long stats::rectArea(int dim) {
    /* Your code here!! */
    return pow(2, dim) * pow(2, dim);
}

// given a rectangle, compute its sum of squared deviations from mean, 
// over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    /* Your code here!! */
    RGBAPixel avg = getAvg(ul, dim);
    double r_var = (getSumSq('r', ul, dim) - (pow(getSum('r', ul, dim), 2)) / rectArea(dim));
    double g_var = (getSumSq('g', ul, dim) - (pow(getSum('g', ul, dim), 2)) / rectArea(dim));
    double b_var = (getSumSq('b', ul, dim) - (pow(getSum('b', ul, dim), 2)) / rectArea(dim));

    return r_var + g_var + b_var;
}

/** Given a square, return the average color value over the square as a pixel.
* Each color component of the pixel is the average value of that component over
* the square.
* @param ul is (x,y) of the upper left corner of the square
* @param dim is log of side length of the square */
RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    /* Your code here!! */
    int length = pow(2, dim);
    int r_avg = (int) (getSum('r', ul, dim) / rectArea(dim));
    int g_avg = (int) (getSum('g', ul, dim) / rectArea(dim));
    int b_avg = (int) (getSum('b', ul, dim) / rectArea(dim));

    return RGBAPixel(r_avg, g_avg, b_avg);
}
