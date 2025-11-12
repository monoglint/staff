#pragma once

/*

modified by monoglint

*/

namespace themes {
    namespace spectrum_dark {
        // Sets the ImGui style to Spectrum
        void load();

        namespace { // Unnamed namespace, since we only use this here. 
            unsigned int color(unsigned int c) {
                // add alpha.
                // also swap red and blue channel for some reason.
                // todo: figure out why, and fix it.
                const short a = 0xFF;
                const short r = (c >> 16) & 0xFF;
                const short g = (c >> 8) & 0xFF;
                const short b = (c >> 0) & 0xFF;
                return(a << 24)
                    | (r << 0)
                    | (g << 8)
                    | (b << 16);
            }
        }
        // all colors are from http://spectrum.corp.adobe.com/color.html

        inline unsigned int color_alpha(unsigned int alpha, unsigned int c) {
            return ((alpha & 0xFF) << 24) | (c & 0x00FFFFFF);
        }

        namespace static_colors { // static colors
            const unsigned int NONE = 0x00000000; // transparent
            const unsigned int WHITE = color(0xFFFFFF);
            const unsigned int BLACK = color(0x000000);
            const unsigned int GRAY200 = color(0xF4F4F4);
            const unsigned int GRAY300 = color(0xEAEAEA);
            const unsigned int GRAY400 = color(0xD3D3D3);
            const unsigned int GRAY500 = color(0xBCBCBC);
            const unsigned int GRAY600 = color(0x959595);
            const unsigned int GRAY700 = color(0x767676);
            const unsigned int GRAY800 = color(0x505050);
            const unsigned int GRAY900 = color(0x323232);
            const unsigned int BLUE400 = color(0x378EF0);
            const unsigned int BLUE500 = color(0x2680EB);
            const unsigned int BLUE600 = color(0x1473E6);
            const unsigned int BLUE700 = color(0x0D66D0);
            const unsigned int RED400 = color(0xEC5B62);
            const unsigned int RED500 = color(0xE34850);
            const unsigned int RED600 = color(0xD7373F);
            const unsigned int RED700 = color(0xC9252D);
            const unsigned int ORANGE400 = color(0xF29423);
            const unsigned int ORANGE500 = color(0xE68619);
            const unsigned int ORANGE600 = color(0xDA7B11);
            const unsigned int ORANGE700 = color(0xCB6F10);
            const unsigned int GREEN400 = color(0x33AB84);
            const unsigned int GREEN500 = color(0x2D9D78);
            const unsigned int GREEN600 = color(0x268E6C);
            const unsigned int GREEN700 = color(0x12805C);
        }

        const unsigned int GRAY50  = color(0x1F262F);
        const unsigned int GRAY75  = color(0x272F3A);
        const unsigned int GRAY100 = color(0x2C343F);
        const unsigned int GRAY200 = color(0x333B47);
        const unsigned int GRAY300 = color(0x3A4350);
        const unsigned int GRAY400 = color(0x495563);
        const unsigned int GRAY500 = color(0x576271);
        const unsigned int GRAY600 = color(0x737D8B);
        const unsigned int GRAY700 = color(0x8F99A5);
        const unsigned int GRAY800 = color(0xC1C8D2);
        const unsigned int GRAY900 = color(0xF2F5FA);


        const unsigned int BLUE400 = color(0x2680EB);
        const unsigned int BLUE500 = color(0x378EF0);
        const unsigned int BLUE600 = color(0x4B9CF5);
        const unsigned int BLUE700 = color(0x5AA9FA);
        const unsigned int RED400 = color(0xE34850);
        const unsigned int RED500 = color(0xEC5B62);
        const unsigned int RED600 = color(0xF76D74);
        const unsigned int RED700 = color(0xFF7B82);
        const unsigned int ORANGE400 = color(0xE68619);
        const unsigned int ORANGE500 = color(0xF29423);
        const unsigned int ORANGE600 = color(0xF9A43F);
        const unsigned int ORANGE700 = color(0xFFB55B);
        const unsigned int GREEN400 = color(0x2D9D78);
        const unsigned int GREEN500 = color(0x33AB84);
        const unsigned int GREEN600 = color(0x39B990);
        const unsigned int GREEN700 = color(0x3FC89C);
        const unsigned int INDIGO400 = color(0x6767EC);
        const unsigned int INDIGO500 = color(0x7575F1);
        const unsigned int INDIGO600 = color(0x8282F6);
        const unsigned int INDIGO700 = color(0x9090FA);
        const unsigned int CELERY400 = color(0x44B556);
        const unsigned int CELERY500 = color(0x4BC35F);
        const unsigned int CELERY600 = color(0x51D267);
        const unsigned int CELERY700 = color(0x58E06F);
        const unsigned int MAGENTA400 = color(0xD83790);
        const unsigned int MAGENTA500 = color(0xE2499D);
        const unsigned int MAGENTA600 = color(0xEC5AAA);
        const unsigned int MAGENTA700 = color(0xF56BB7);
        const unsigned int YELLOW400 = color(0xDFBF00);
        const unsigned int YELLOW500 = color(0xEDCC00);
        const unsigned int YELLOW600 = color(0xFAD900);
        const unsigned int YELLOW700 = color(0xFFE22E);
        const unsigned int FUCHSIA400 = color(0xC038CC);
        const unsigned int FUCHSIA500 = color(0xCF3EDC);
        const unsigned int FUCHSIA600 = color(0xD951E5);
        const unsigned int FUCHSIA700 = color(0xE366EF);
        const unsigned int SEAFOAM400 = color(0x1B959A);
        const unsigned int SEAFOAM500 = color(0x20A3A8);
        const unsigned int SEAFOAM600 = color(0x23B2B8);
        const unsigned int SEAFOAM700 = color(0x26C0C7);
        const unsigned int CHARTREUSE400 = color(0x85D044);
        const unsigned int CHARTREUSE500 = color(0x8EDE49);
        const unsigned int CHARTREUSE600 = color(0x9BEC54);
        const unsigned int CHARTREUSE700 = color(0xA3F858);
        const unsigned int PURPLE400 = color(0x9256D9);
        const unsigned int PURPLE500 = color(0x9D64E1);
        const unsigned int PURPLE600 = color(0xA873E9);
        const unsigned int PURPLE700 = color(0xB483F0);
    }
}