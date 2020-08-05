var HID = require('node-hid');
var hid = new HID.HID(2064, 58625)

function sleep(ms) {
    return new Promise((resolve) => {
      setTimeout(resolve, ms);
    });
  } 


hid.on("data", function(data) {
    // if (!(data[0] === 1 
    //     && data[1] === 128 && data[2] === 128 
    //     && data[3] === 127 && data[4] === 127
    //     && data[5] === 15 
    //     && data[6] === 0 && data[7] === 0)){
    //         console.log(data);
    // }

    if (data[4] === 0){
        console.log("Cima")
    }
    if (data[4] === 255){
        console.log("Baixo")
    }
    if (data[3] === 255){
        console.log("Direita")
    }
    if (data[3] === 0){
        console.log("Esquerda")
    }
    if (data[5] === 31){
        console.log("B")
    }
    if (data[5] === 47){
        console.log("A")
    }
});

// Cima data[4] === 0
// Baixo data[4] === 255
// Direita data[3] === 255
// Esquerda data[3] === 0

// B data[5] === 31
// A data[5] === 47

// Start data[6] === 32
// Select data[6] === 16