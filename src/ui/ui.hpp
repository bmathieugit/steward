#ifndef __steward_ui_ui_hpp__
#define __steward_ui_ui_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/exception.hpp>
#include <core/file.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>

namespace ui {
/**
 * ce namespace va contenir l'ensemble des fonctionnalité UI de steward.
 * on commencera par définir la notion de window.
 * Puis on verra la notion de component.
 * Les component repondront à des event clavier dans un premier temps et souris
 * plus tard. Il faut donc un systeme de gestion évenementiel.
 */

/**
 * On va commencer par implementer la logique de window. Une window se coompose
 * d'une matrix N*M
 */

class pixel_out_of_screen {
 public:
  const char* message() { return "pixel_out_of_screen exception"; }
};

struct color {
  enum class palette : u8 {
    black = 0,
    red = 1,
    green = 2,
    yellow = 3,
    blue = 4,
    magenta = 5,
    cyan = 6,
    graylight = 7,
    graydark = 8,
    redlight = 9,
    greenlight = 10,
    yellowlight = 11,
    bluelight = 12,
    magentalight = 13,
    cyanlight = 14,
    white = 15,
    aquamarine1 = 122,
    aquamarine1bis = 86,
    aquamarine3 = 79,
    blue1 = 21,
    blue3 = 19,
    blue3bis = 20,
    blueviolet = 57,
    cadetblue = 72,
    cadetbluebis = 73,
    chartreuse1 = 118,
    chartreuse2 = 112,
    chartreuse2bis = 82,
    chartreuse3 = 70,
    chartreuse3bis = 76,
    chartreuse4 = 64,
    cornflowerblue = 69,
    cornsilk1 = 230,
    cyan1 = 51,
    cyan2 = 50,
    cyan3 = 43,
    darkblue = 18,
    darkcyan = 36,
    darkgoldenrod = 136,
    darkgreen = 22,
    darkkhaki = 143,
    darkmagenta = 90,
    darkmagentabis = 91,
    darkolivegreen1 = 191,
    darkolivegreen1bis = 192,
    darkolivegreen2 = 155,
    darkolivegreen3 = 107,
    darkolivegreen3bis = 113,
    darkolivegreen3ter = 149,
    darkorange = 208,
    darkorange3 = 130,
    darkorange3bis = 166,
    darkred = 52,
    darkredbis = 88,
    darkseagreen = 108,
    darkseagreen1 = 158,
    darkseagreen1bis = 193,
    darkseagreen2 = 151,
    darkseagreen2bis = 157,
    darkseagreen3 = 115,
    darkseagreen3bis = 150,
    darkseagreen4 = 65,
    darkseagreen4bis = 71,
    darkslategray1 = 123,
    darkslategray2 = 87,
    darkslategray3 = 116,
    darkturquoise = 44,
    darkviolet = 128,
    darkvioletbis = 92,
    deeppink1 = 198,
    deeppink1bis = 199,
    deeppink2 = 197,
    deeppink3 = 161,
    deeppink3bis = 162,
    deeppink4 = 125,
    deeppink4bis = 89,
    deeppink4ter = 53,
    deepskyblue1 = 39,
    deepskyblue2 = 38,
    deepskyblue3 = 31,
    deepskyblue3bis = 32,
    deepskyblue4 = 23,
    deepskyblue4bis = 24,
    deepskyblue4ter = 25,
    dodgerblue1 = 33,
    dodgerblue2 = 27,
    dodgerblue3 = 26,
    gold1 = 220,
    gold3 = 142,
    gold3bis = 178,
    green1 = 46,
    green3 = 34,
    green3bis = 40,
    green4 = 28,
    greenyellow = 154,
    grey0 = 16,
    grey100 = 231,
    grey11 = 234,
    grey15 = 235,
    grey19 = 236,
    grey23 = 237,
    grey27 = 238,
    grey3 = 232,
    grey30 = 239,
    grey35 = 240,
    grey37 = 59,
    grey39 = 241,
    grey42 = 242,
    grey46 = 243,
    grey50 = 244,
    grey53 = 102,
    grey54 = 245,
    grey58 = 246,
    grey62 = 247,
    grey63 = 139,
    grey66 = 248,
    grey69 = 145,
    grey7 = 233,
    grey70 = 249,
    grey74 = 250,
    grey78 = 251,
    grey82 = 252,
    grey84 = 188,
    grey85 = 253,
    grey89 = 254,
    grey93 = 255,
    honeydew2 = 194,
    hotpink = 205,
    hotpink2 = 169,
    hotpink3 = 132,
    hotpink3bis = 168,
    hotpinkbis = 206,
    indianred = 131,
    indianred1 = 203,
    indianred1bis = 204,
    indianredbis = 167,
    khaki1 = 228,
    khaki3 = 185,
    lightcoral = 210,
    lightcyan1bis = 195,
    lightcyan3 = 152,
    lightgoldenrod1 = 227,
    lightgoldenrod2 = 186,
    lightgoldenrod2bis = 221,
    lightgoldenrod2ter = 222,
    lightgoldenrod3 = 179,
    lightgreen = 119,
    lightgreenbis = 120,
    lightpink1 = 217,
    lightpink3 = 174,
    lightpink4 = 95,
    lightsalmon1 = 216,
    lightsalmon3 = 137,
    lightsalmon3bis = 173,
    lightseagreen = 37,
    lightskyblue1 = 153,
    lightskyblue3 = 109,
    lightskyblue3bis = 110,
    lightslateblue = 105,
    lightslategrey = 103,
    lightsteelblue = 147,
    lightsteelblue1 = 189,
    lightsteelblue3 = 146,
    lightyellow3 = 187,
    magenta1 = 201,
    magenta2 = 165,
    magenta2bis = 200,
    magenta3 = 127,
    magenta3bis = 163,
    magenta3ter = 164,
    mediumorchid = 134,
    mediumorchid1 = 171,
    mediumorchid1bis = 207,
    mediumorchid3 = 133,
    mediumpurple = 104,
    mediumpurple1 = 141,
    mediumpurple2 = 135,
    mediumpurple2bis = 140,
    mediumpurple3 = 97,
    mediumpurple3bis = 98,
    mediumpurple4 = 60,
    mediumspringgreen = 49,
    mediumturquoise = 80,
    mediumvioletred = 126,
    mistyrose1 = 224,
    mistyrose3 = 181,
    navajowhite1 = 223,
    navajowhite3 = 144,
    navyblue = 17,
    orange1 = 214,
    orange3 = 172,
    orange4 = 58,
    orange4bis = 94,
    orangered1 = 202,
    orchid = 170,
    orchid1 = 213,
    orchid2 = 212,
    palegreen1 = 121,
    palegreen1bis = 156,
    palegreen3 = 114,
    palegreen3bis = 77,
    paleturquoise1 = 159,
    paleturquoise4 = 66,
    palevioletred1 = 211,
    pink1 = 218,
    pink3 = 175,
    plum1 = 219,
    plum2 = 183,
    plum3 = 176,
    plum4 = 96,
    purple = 129,
    purple3 = 56,
    purple4 = 54,
    purple4bis = 55,
    purplebis = 93,
    red1 = 196,
    red3 = 124,
    red3bis = 160,
    rosybrown = 138,
    royalblue1 = 63,
    salmon1 = 209,
    sandybrown = 215,
    seagreen1 = 84,
    seagreen1bis = 85,
    seagreen2 = 83,
    seagreen3 = 78,
    skyblue1 = 117,
    skyblue2 = 111,
    skyblue3 = 74,
    slateblue1 = 99,
    slateblue3 = 61,
    slateblue3bis = 62,
    springgreen1 = 48,
    springgreen2 = 42,
    springgreen2bis = 47,
    springgreen3 = 35,
    springgreen3bis = 41,
    springgreen4 = 29,
    steelblue = 67,
    steelblue1 = 75,
    steelblue1bis = 81,
    steelblue3 = 68,
    tan = 180,
    thistle1 = 225,
    thistle3 = 182,
    turquoise2 = 45,
    turquoise4 = 30,
    violet = 177,
    wheat1 = 229,
    wheat4 = 101,
    yellow1 = 226,
    yellow2 = 190,
    yellow3 = 148,
    yellow3bis = 184,
    yellow4 = 100,
    yellow4bis = 106,
  };

  palette _value = palette::black;
};

static constexpr color basic_background = {color::palette::black};
static constexpr color basic_foreground = {color::palette::white};

struct pixel {
  bool _bold = false;
  bool _underlined = false;

  color _foreground = basic_foreground;
  color _background = basic_background;

  char _content;
};

struct dimensions {
  size_t _y = 0;
  size_t _x = 0;
};

struct cursor {
  size_t _x = 0;
  size_t _y = 0;

  enum class shape : u8 {
    hidden = 0,
    block_blinking = 1,
    block = 2,
    underline_blinking = 3,
    underline = 4,
    bar_blinking = 5,
    bar = 6,
  };

  shape _shape = shape::block;
};

class screen {
 private:
  cursor _cursor;
  dimensions _dims;
  vector<pixel> _pixels;

 public:
  constexpr screen(dimensions dims)
      : _dims(dims), _pixels(dims._y * dims._x, pixel()) {}

  constexpr const pixel& at(size_t x, size_t y) const {
    if (x < _dims._x and y < _dims._y) {
      return _pixels.at(y + y * x);
    }

    else {
      throw pixel_out_of_screen();
    }
  }

  constexpr pixel& at(size_t x, size_t y) {
    if (x < _dims._x and y < _dims._y) {
      return _pixels.at(y + y * x);
    }

    else {
      throw pixel_out_of_screen();
    }
  }

  constexpr void clear() {
    for_each([](pixel& p) { p = pixel(); })(iter(_pixels));
    _cursor._x = 0;
    _cursor._y = 0;
  }

  constexpr auto dims() const { return _dims; }
};

void render(oterator auto o, const pixel& px) {
  constexpr array<const char*, 33> palette16code = {
      "30m", "40m",   //
      "31m", "41m",   //
      "32m", "42m",   //
      "33m", "43m",   //
      "34m", "44m",   //
      "35m", "45m",   //
      "36m", "46m",   //
      "37m", "47m",   //
      "90m", "100m",  //
      "91m", "101m",  //
      "92m", "102m",  //
      "93m", "103m",  //
      "94m", "104m",  //
      "95m", "105m",  //
      "96m", "106m",  //
      "97m", "107m",  //
  };

  if (px._bold) {
    write(o, "\033[1m");
  }

  if (px._underlined) {
    write(o, "\033[4m");
  }

  write(o, "\033[",
        palette16code.at(static_cast<u8>(px._foreground._value) * 2));
  write(o, "\033[", palette16code.at(static_cast<u8>(px._background._value)));

  write(o, px._content);
  write(o, "\033[0m");
}

void render(oterator auto o, const screen& scr) {
  for (size_t y = 0; y < scr.dims()._y; ++y) {
    for (size_t x = 0; x < scr.dims()._x; ++x) {
      auto& px = scr.at(x, y);
      render(o, px);
    }
    write(o, '\n');
  }
}
}  // namespace ui

#endif