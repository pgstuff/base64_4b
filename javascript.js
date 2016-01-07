var digitsStr =
//   0       8       16      24      32      40      48      56     63
//   v       v       v       v       v       v       v       v      v
    "+-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
var digits = digitsStr.split('');
var digitsMap = {};
for (var i = 0; i < digits.length; i++) {
    digitsMap[digits[i]] = i;
}

uint32=4294967295;
var result = '';
while (true) {
    result = digits[uint32 & 0x3f] + result;
    uint32 >>>= 6;
    if (uint32 === 0)
        break;
}
alert(result);

digitsStr = 'zzzzz';
var result = 0;
var digits = digitsStr.split('');
for (var i = 0; i < digits.length; i++) {
    result = (result << 6) + digitsMap[digits[i]];
}
alert(result);
