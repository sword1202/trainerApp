function floatModulo(x, divider) {
    var division = x / divider
    if (division < divider) {
        return x
    }

    return division - Math.floor(division)
}
