#pragma once
#include <string>
#include <stdexcept>
using namespace std;

pair<string,string> splitDecimal(const string& s){
    size_t pos = s.find('.');
    if(pos == string::npos) return {s,"0"};
    return {s.substr(0,pos), s.substr(pos+1)};
}

string removeLeadingZeros(const string& s){
    if(s.empty()) return "0";
    size_t k=0;
    while(k+1 < s.size() && s[k]=='0') k++;
    return s.substr(k);
}

string removeTrailingZeros(const string& s){
    string r = s;
    while(!r.empty() && r.back()=='0') r.pop_back();
    if(r.empty()) r="0";
    return r;
}

string padRight(const string& s, size_t n){
    string r = s;
    while(r.size()<n) r+='0';
    return r;
}

string padLeft(const string& s, size_t n){
    string r = s;
    while(r.size()<n) r='0'+r;
    return r;
}

int compare(const string& a, const string& b){
    auto [aInt,aFrac] = splitDecimal(a);
    auto [bInt,bFrac] = splitDecimal(b);
    aInt = removeLeadingZeros(aInt);
    bInt = removeLeadingZeros(bInt);
    if(aInt.size()!=bInt.size()) return (aInt.size()<bInt.size())?-1:1;
    for(size_t i=0;i<aInt.size();i++){
        if(aInt[i]<bInt[i]) return -1;
        if(aInt[i]>bInt[i]) return 1;
    }
    size_t len = max(aFrac.size(), bFrac.size());
    aFrac = padRight(aFrac,len);
    bFrac = padRight(bFrac,len);
    for(size_t i=0;i<len;i++){
        if(aFrac[i]<bFrac[i]) return -1;
        if(aFrac[i]>bFrac[i]) return 1;
    }
    return 0;
}

bool isNegative(const string& s){ return !s.empty() && s[0]=='-'; }

string normalize(const string& s) {
    if(s.empty()) return string("0");
    bool neg = false;
    string t = s;
    if(t[0]=='-'){ neg = true; t = t.substr(1); }
    auto [i,f] = splitDecimal(t);
    i = removeLeadingZeros(i);
    f = removeTrailingZeros(f);
    string res;
    if(f == "0" || f.empty()) res = i;
    else res = i + "." + f;
    if(res.empty()) res = "0";
    if(res == "0") neg = false;
    if(neg) res = "-" + res;
    return res;
}

string sum(const string& a, const string& b){
    auto [aInt,aFrac] = splitDecimal(a);
    auto [bInt,bFrac] = splitDecimal(b);
    size_t lenFrac = max(aFrac.size(), bFrac.size());
    aFrac = padRight(aFrac,lenFrac);
    bFrac = padRight(bFrac,lenFrac);

    string fracSum(lenFrac,'0');
    int carry=0;
    for(int i=(int)lenFrac-1;i>=0;i--){
        int tmp = (aFrac[i]-'0')+(bFrac[i]-'0')+carry;
        fracSum[i]=tmp%10+'0';
        carry=tmp/10;
    }

    size_t lenInt = max(aInt.size(), bInt.size());
    aInt = padLeft(aInt,lenInt);
    bInt = padLeft(bInt,lenInt);
    string intSum(lenInt,'0');
    for(int i=(int)lenInt-1;i>=0;i--){
        int tmp = (aInt[i]-'0') + (bInt[i]-'0') + carry;
        intSum[i] = tmp%10+'0';
        carry = tmp/10;
    }
    string res = (carry?"1":"") + intSum;
    fracSum = removeTrailingZeros(fracSum);
    if(fracSum!="0") res = res + "." + fracSum;
    return normalize(res);
}

string diff(const string& a, const string& b){
    if(compare(a,b)<0) return string("-") + diff(b,a);
    auto [aInt,aFrac] = splitDecimal(a);
    auto [bInt,bFrac] = splitDecimal(b);
    size_t lenFrac = max(aFrac.size(), bFrac.size());
    aFrac = padRight(aFrac,lenFrac);
    bFrac = padRight(bFrac,lenFrac);

    string fracRes(lenFrac,'0');
    int carry=0;
    for(int i=(int)lenFrac-1;i>=0;i--){
        int tmp = (aFrac[i]-'0') - (bFrac[i]-'0') - carry;
        if(tmp<0){ tmp+=10; carry=1; } else carry=0;
        fracRes[i]=tmp+'0';
    }

    size_t lenInt = max(aInt.size(), bInt.size());
    aInt = padLeft(aInt,lenInt);
    bInt = padLeft(bInt,lenInt);
    string intRes(lenInt,'0');
    for(int i=(int)lenInt-1;i>=0;i--){
        int tmp = (aInt[i]-'0') - (bInt[i]-'0') - carry;
        if(tmp<0){ tmp+=10; carry=1; } else carry=0;
        intRes[i]=tmp+'0';
    }
    fracRes = removeTrailingZeros(fracRes);
    string res = removeLeadingZeros(intRes);
    if(fracRes!="0") res += "."+fracRes;
    return normalize(res);
}

string multi(const string& a, const string& b){
    auto [aInt,aFrac] = splitDecimal(a);
    auto [bInt,bFrac] = splitDecimal(b);
    size_t decLen = aFrac.size() + bFrac.size();

    string numA = aInt + aFrac;
    string numB = bInt + bFrac;
    string res(numA.size()+numB.size(),'0');
    for(int i=(int)numA.size()-1;i>=0;i--){
        int carry=0;
        for(int j=(int)numB.size()-1;j>=0;j--){
            int tmp = (numA[i]-'0')*(numB[j]-'0') + (res[i+j+1]-'0') + carry;
            res[i+j+1] = tmp%10+'0';
            carry = tmp/10;
        }
        res[i] += carry;
    }
    size_t k=0; while(k<res.size()-1 && res[k]=='0') k++;
    res = res.substr(k);
    if(decLen>0){
        if(res.size()<=decLen) res = string(decLen-res.size()+1,'0') + res;
        res.insert(res.end()-decLen,'.');
        res = removeTrailingZeros(res);
    }
    return normalize(res);
}

// FIXED division
string divLong(string a, string b, int prec = 20) {
    bool neg = false;
    if (!a.empty() && a[0] == '-') { neg = !neg; a.erase(0,1); }
    if (!b.empty() && b[0] == '-') { neg = !neg; b.erase(0,1); }

    size_t p;
    int decA = 0, decB = 0;
    if ((p = a.find('.')) != string::npos) { decA = (int)(a.size() - p - 1); a.erase(p,1); }
    if ((p = b.find('.')) != string::npos) { decB = (int)(b.size() - p - 1); b.erase(p,1); }

    if (b == "0") throw runtime_error("Division by zero");

    int totalDec = prec + decB - decA; // FIX
    if (totalDec < 0) totalDec = 0;
    a.append((size_t)totalDec, '0');

    string cur = "";
    string q = "";
    for (size_t i = 0; i < a.size(); ++i) {
        if (!(cur.size() == 1 && cur[0] == '0')) cur.push_back(a[i]); else cur[0] = a[i];
        while (cur.size() > 1 && cur[0] == '0') cur.erase(0,1);

        int lo = 0, hi = 9, digit = 0;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            string prod = multi(b, string(1, char('0' + mid)));
            int cmp = compare(prod, cur);
            if (cmp <= 0) { digit = mid; lo = mid + 1; } else hi = mid - 1;
        }
        q.push_back(char('0' + digit));
        if (digit) cur = diff(cur, multi(b, string(1, char('0' + digit))));
    }

    size_t k = 0; while (k + 1 < q.size() && q[k] == '0') ++k;
    q = q.substr(k);

    if (prec > 0) {
        if (q.size() <= (size_t)prec) q = string((size_t)prec - q.size() + 1, '0') + q;
        q.insert(q.size() - prec, ".");
    }

    while (!q.empty() && q.back() == '0') q.pop_back();
    if (!q.empty() && q.back() == '.') q.pop_back();
    if (q.empty()) q = "0";

    if (neg && q != "0") q = "-" + q;
    return normalize(q);
}

inline string divisor(const string& s, const string& b, int prec = 20) { return divLong(s,b,prec); }
inline string divisor(const string& s, int b, int prec = 20) { return divLong(s,to_string(b),prec); }

string roundTo(const string& s, int prec) {
    string t = normalize(s);
    auto [i,f] = splitDecimal(t);
    if (prec <= 0) return i;
    if ((int)f.size() > prec) f = f.substr(0, prec);
    f = removeTrailingZeros(f);
    if (f == "0" || f.empty()) return i;
    return i + "." + f;
}

string half(const string& s, int prec = 20) {
    string t = normalize(s);
    if (t == "0") return string("0");
    bool neg = false;
    if (t[0] == '-') { neg = true; t = t.substr(1); }
    auto [i, f] = splitDecimal(t);
    if(i.empty()) i = "0";
    string resInt;
    resInt.reserve(i.size());
    int carry = 0;
    for (size_t idx = 0; idx < i.size(); ++idx) {
        int cur = carry*10 + (i[idx]-'0');
        resInt.push_back(char('0' + (cur/2)));
        carry = cur % 2;
    }
    string resFrac;
    for (size_t idx = 0; idx < f.size(); ++idx) {
        int cur = carry*10 + (f[idx]-'0');
        resFrac.push_back(char('0' + (cur/2)));
        carry = cur % 2;
    }
    int target = prec;
    while ((int)resFrac.size() < target && carry != 0) {
        int cur = carry*10;
        resFrac.push_back(char('0' + (cur/2)));
        carry = cur % 2;
    }
    resInt = removeLeadingZeros(resInt);
    if (resInt.empty()) resInt = "0";
    resFrac = removeTrailingZeros(resFrac);
    string out;
    if (resFrac == "0" || resFrac.empty()) out = resInt;
    else out = resInt + "." + resFrac;
    if (neg && out != "0") out = "-" + out;
    return normalize(out);
}
