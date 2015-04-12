
function draw(arrayOfRanges) {
    // Set up!
    var a_canvas = document.getElementById("a");
    var context = a_canvas.getContext("2d");
    context.clearRect ( 0 , 0 , canvas.width, canvas.height );

    var x0=300
    var y0=300
    context.fillRect(x0, y0, 4, 4);

    var arrayLength = arrayOfRanges.length;
    for (var i = 0; i < arrayLength; i++) {
        var range = arrayOfRanges[i];

        var angle = 360/arrayLength*i
        context.fillStyle = "#F00";
        context.fillRect(x0 + range * Math.cos(Math.PI * angle / 180.0),
                         y0 + range * Math.sin(Math.PI * angle / 180.0), 4, 4);
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

function load() {
    $.ajax({
        type:'get',
        url: "points",
        success: function (arr) {
            draw(arr)
        }
    });
}

setInterval(function () {load()}, 1000);
