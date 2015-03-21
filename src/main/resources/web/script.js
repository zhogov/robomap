
function draw(arrayOfPoints) {
    // Set up!
    var a_canvas = document.getElementById("a");
    var context = a_canvas.getContext("2d");

    var arrayLength = arrayOfPoints.length;
    for (var i = 0; i < arrayLength; i++) {
        var point = arrayOfPoints[i];
        context.fillStyle = "#F00";
        context.fillRect(point.x, point.y, 4, 4);
    }

// Draw the face
    context.fillStyle = "yellow";
    context.beginPath();
    context.arc(95, 85, 40, 0, 2*Math.PI);
    context.closePath();
    context.fill();
    context.lineWidth = 2;
    context.stroke();
    context.fillStyle = "black";

// Draw the left eye
    context.beginPath();
    context.arc(75, 75, 5, 0, 2*Math.PI);
    context.closePath();
    context.fill();

// Draw the right eye
    context.beginPath();
    context.arc(114, 75, 5, 0, 2*Math.PI);
    context.closePath();
    context.fill();

// Draw the mouth
    context.beginPath();
    context.arc(95, 90, 26, Math.PI, 2*Math.PI, true);
    context.closePath();
    context.fill();

// Write "Hello, World!"
    context.font = "30px Garamond";
    context.fillText("Hello, World!",15,175);
}

var arr = [
    {"x":50, "y":50},
    {"x":150, "y":50},
    {"x":250, "y":150},
    {"x":350, "y":250},
    {"x":450, "y":50}
];

function load() {
    $.ajax({
        type:'get',
        url: "/azaza",
        success: function (arr) {
            draw(arr)
        }
    });
}

setInterval(function () {load()}, 1000);
