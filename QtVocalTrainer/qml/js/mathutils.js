function floatModulo(x, divider) {
    console.log("floatModule")
    var division = x / divider
    console.log("division = " + division)
    return division - Math.floor(division)
}
