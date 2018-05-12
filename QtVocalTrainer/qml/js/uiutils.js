function destroyAllChildern(parent) {
    var children = parent.children
    var length = children.length
    for (var i = 0; i < length; i++) {
        var child = children[i]
        child.visible = false
        child.destroy()
        delete children[i]
    }
}
