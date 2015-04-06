#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include <exception>

const char* test_str = " mr vy! mt rQ h[ kF aw kE\" Zw kG T@ q] T@ ~I\" T@ AKA Zv AQi ai AQk\" h[ AQi m"
                       "t AJX mr ADw\"# nI AZ[! nI Avp$ |C Avp$ |C ^h$ mk ^h$ mk bl$ l` a` gc \\U _F "
                       "\\U\" VR \\T Fa cj Fa ~I\" Fa ATf RS A`M `S A`M\" e_ A`M je A^W nI AZ[\"#% !% B\\"
                       "P$  B\\P$  $#& '% !% B\\P$  B\\P$  $# ZK ^h! ZK A]p$ hO A]p$ hO ^h$# ZK AfV!"
                       " ZK Au~$ hO Au~$ hO AfV$#& ' l[ AE~! l[ AKe fG AQX `Q AQX\" [O AQX S] ANK S] "
                       "?t\" S] pa ]A nR `L nR\" f_ nR l[ rg l[ yS\"#  B\\P!% B\\P$% $  $# ld AWi! kG A"
                       "Yn fV A^\\ _b A^\\\" T} A^[ FM AXT FM }s\" FN hy V{ ax ]r ax\" eL aw jl fK lL g"
                       "s\" lL aN$ lL \\W gM Wg ^w Wg\" Wk Wh V{ \\O V{ ^a\" HO ^a$ HO WN L| Ld ]~ Lc\" "
                       "mN Lc rP RX t[ Td\" vP VZ x? [^ x? _a\" x? A]p$ le A]p$#& '% !% B\\P$  B\\P$  "
                       "$# ZK ^h! ZK A]p$ hO A]p$ hO ^h$# ZK AfV! ZK Au~$ hO Au~$ hO AfV$#& ' dX Aue"
                       "! Wa Aue$ Wa A^w$ Pr A^w$ Pr ATT$ Wa ATT$ Wa ld$ Wa d? [Z _B fP _C\" kU _C kH"
                       " _A ob _r\" ob lz$ lj lZ kq lM jW lP\" gj lU dX mR dX rF\" dX ATS$ nd ATS$ nd A"
                       "^w$ dX A^w$#% !  $  B\\P$% B\\P$#& '  !  B\\P$% B\\P$% $# J{ AIx! V~ AIx$ V~"
                       " APR ZR ASi `f ASi\" jj ASj jU AOK jT AId\" dF AGI dk AGM [L AEC\" OI ABQ Gq }G"
                       " Gp ph\" Gq d[ P] ]z ZP ]{\" dD ]z fF aE jJ cr\" jJ ^z$ yb ^z$ uz dp vw ey vu j"
                       "R\" vv mn vu AOX vu AOX\" vv AVC sX AZH qG A[_\" k] A^w d^ A_Q `f A_R\" Ru A_P J"
                       "z AXU J{ AIx\"# jT }j! jT uI$ jT qP ee in \\R im\" Wp il UN mC UM qZ\" UN ur X{"
                       " yI \\D yq\" _U z[ fv |V jT }j\"#& '% !% B\\P$  B\\P$  $# ZK ^h! ZK Awb$ hO Aw"
                       "b$ hO ^h$#";
const char* traslates[] = { "0","moveto","curveto","closepath","lineto","boxw","boxstep","traslate" , "<ERROR>" };

int decode_it(std::stringstream& line, std::stack<unsigned int>& stack, const std::string& word) {
     int tabs = 0;
    if(word == "boxw") {
        line << "4250 round-to-pixels";
        tabs = 2;
        stack.push(4250);
    } else if(word == "boxstep") {
        line << "4250 round-to-pixels";
        tabs = 2;
         stack.push(4950);
    } else if(word == "lineto" || word =="moveto") {
        line << stack.top() << "\t";
        stack.pop();
        line << stack.top() << "\t" << word;
        stack.pop();
        tabs = 2;
    } else if (word == "traslate") {
        line << stack.top() << "\t";
        stack.pop();
        line << stack.top() << "\t" << word;
        stack.pop();
        tabs = 3;
    } else if(word == "curveto") {
        for(int i=0;i<6;i++) {
                line << stack.top() << "\t";
                stack.pop();
        }
        line << word;
        tabs = 6;

    } else if(word == "closepath") {
        line << "closepath";
        tabs = 1;
    } else {
        throw std::exception("crap something went wrong");
    }
   return tabs;
}

void __stdcall makefile(const char* filename) {

    std::stringstream out_text;
    std::string char_data;
    size_t len = strlen(test_str);
    std::stack<unsigned int> stack;

    for(size_t i=0;i < len;i++) {
        stack.push(test_str[i]);
        char_data.push_back(test_str[i]);
     //   current.append(test_str[i]);
        if(stack.top() > 62) {
            unsigned int a = stack.top() & 63; stack.pop();
            unsigned int b = stack.top();stack.pop();
             stack.push(a + (b << 6));
        } else {
            if(stack.top() >=32) {
                unsigned index = stack.top() -32; stack.pop();
                if(index == 0) stack.push(0);
                else {
                    const char* word = traslates[index];
                    int tabs = decode_it(out_text,stack,word);
                    for(; tabs < 9; tabs++) out_text << '\t';
                    out_text << "# -[" << char_data << "]-" << std::endl;
                    char_data.clear();
                }
                // pop
            } else {
                stack.pop();
            }
        }
   }
    std::fstream f;
    f.open(filename,std::ios::out);
    f << out_text.str();
    f.close();
}
