
var game = new Phaser.Game(800, 500, Phaser.AUTO, 'phaser', { preload: preload, create: create, update: update});

var player;
var otherPlayers = {};

function preload() {
    this.load.image('player', '/assets/ball.png');
}

function create() {

    this.client = new Client();
    this.client.openConnection();

    player = this.add.sprite(0,0,'player');
    this.physics.arcade.enableBody(player);
    player.body.collideWorldBounds = true;
    this.cursors = this.input.keyboard.createCursorKeys();
}

function update() {
    //Shitty basic movement
    var hSpeed = 0;
    var vSpeed = 0;

    var moveSpeed = 250;

    if(this.cursors.left.isDown)
    {
        hSpeed -= 1;
    }

    if(this.cursors.right.isDown)
    {
        hSpeed += 1;
    }

    if(this.cursors.up.isDown)
    {
        vSpeed -= 1;
    }

    if(this.cursors.down.isDown)
    {
        vSpeed += 1;
    }
    player.body.velocity.x = hSpeed * moveSpeed;
    player.body.velocity.y = vSpeed * moveSpeed;

    if(this.client.connected)
    {
        this.client.sendMessage(player.body.position);
    }
}

function Client() {

}

Client.prototype.openConnection = function() {
    this.ws = new WebSocket("ws://127.0.0.1:9001");
    this.connected = false;
    this.ws.binaryType = "arraybuffer";
    this.ws.onmessage = this.onMessage.bind(this);
    this.ws.onerror = this.displayError.bind(this);
    this.ws.onopen = this.connectionOpen.bind(this);
};

Client.prototype.sendMessage = function(payload) {

    var buffer = new Uint32Array(2);
    buffer[0] = payload.x;
    buffer[1] = payload.y;
    this.ws.send(buffer);
};

Client.prototype.connectionOpen = function() {
    this.connected = true;
    this.sendMessage(player.body.position);
};

Client.prototype.onMessage = function(message) {
    if(typeof message.data == 'string')
    {
        //text message (connections)
        var text = message.data;
        if(text.startsWith('current_connections'))
        {
            text = text.slice('current_connections'.length + 1);
            otherPlayers
        }
        else if(text.startsWith('player_id'))
        {
            text = text.slice('player_id'.length + 1);
            player.id = text;
        }
        else if(text.startsWith('new_connection'))
        {
            text = text.slice('new_connection'.length + 1);
            console.log(text);
        }
    }
    else
    {
        //binary message (position updates)
        var xBuffer = new Int32Array(message.data, 0, 1);
        var yBuffer = new Int32Array(message.data, 4, 1);
    }
};

Client.prototype.displayError = function(err) {
    console.log('Websocketerror: ' + err);
};
