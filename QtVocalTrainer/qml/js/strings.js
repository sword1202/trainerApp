String.prototype.removeZerosFromBeginning = function () {
    var s = this;

    while(s.charAt(0) === '0') {
        s = s.substr(1);
    }

    return s;
};
