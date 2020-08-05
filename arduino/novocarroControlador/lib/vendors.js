module.exports = {
    "generic": {
      "vendorId": 2064,
      "productId": 58625,
      "state": {
        "button:A": 0,
        "button:B": 0,
        
        "button:Up": 0,
        "button:Right": 0,
        "button:Down": 0,
        "button:Left": 0,
        
        "button:Start": 0,
        "button:Select": 0
      },
      "prev": {// Simple copy of state
        "button:A": 0,
        "button:B": 0,
        
        "button:Up": 0,
        "button:Right": 0,
        "button:Down": 0,
        "button:Left": 0,
        
        "button:Start": 0,
        "button:Select": 0
      },
      "update": function(data) {
        
        var state = this.state;
        
        state["button:A"] = data[5] === 47;
        state["button:B"] = data[5] === 31;
        
        state["button:Left"] = +(data[3] === 0);
        state["button:Right"] = +(data[3] === 255);
        state["button:Up"] = +(data[4] === 0);
        state["button:Down"] = +(data[4] === 255);
        
        state["button:Start"] = data[6] >> 5 & 1;
        state["button:Select"] = data[6] >> 4 & 1;
        
// Cima data[4] === 0
// Baixo data[4] === 255
// Direita data[3] === 255
// Esquerda data[3] === 0

// B data[5] === 31
// A data[5] === 47

// Start data[6] === 32
// Select data[6] === 16

        return state;
      },
      "setRumble": function() {
        
      },
      "setLED": function(led, val) {
        
      }
    }
  };