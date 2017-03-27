var game = new Phaser.Game(800, 600, Phaser.AUTO, 'phaser', { preload: preload, create: create, update: update});

var player;
var otherPlayers = {};

function preload() {
    this.load.image('player', '/images/knight.png');
    this.load.image('platform_side', '/images/platform_side.png');
    this.load.image('platform_bottom', '/images/platform_bottom.png');
}

function create() {

    this.client = new Client();
    this.client.openConnection();

    game.stage.backgroundColor = "#FFFFFF";

    game.physics.startSystem(Phaser.Physics.ARCADE);

    player = this.add.sprite(game.world.centerX,game.world.centerY,'player');
    player.anchor.setTo(0.5,0.5);
    this.physics.arcade.enableBody(player);
    player.body.collideWorldBounds = true;
    this.cursors = this.input.keyboard.createCursorKeys();

    platforms = game.add.group();
    platforms.enableBody = true;
    var bottom = platforms.create(200,100, 'platform_bottom');
    bottom.body.immovable = true;
    bottom = platforms.create(200,468, 'platform_bottom');
    bottom.body.immovable = true;
    var side = platforms.create(200,100, 'platform_side');
    side.body.immovable = true;
    side = platforms.create(570,100, 'platform_side');
    side.body.immovable = true;
}

function update() {
    //Shitty basic movement
    var hSpeed = 0;
    var vSpeed = 0;

    var moveSpeed = 250;

    var hitPlatform = game.physics.arcade.collide(player, platforms);
    
    if(game.input.keyboard.isDown(Phaser.Keyboard.LEFT))
    {
        hSpeed -= 1;
        player.angle = 90;
    }

    if(game.input.keyboard.isDown(Phaser.Keyboard.RIGHT))
    {
        hSpeed += 1;
        player.angle = -90;
    }

    if(game.input.keyboard.isDown(Phaser.Keyboard.UP))
    {
        vSpeed -= 1;
        player.angle = -180;
    }

    if(game.input.keyboard.isDown(Phaser.Keyboard.DOWN))
    {
        vSpeed += 1;
        player.angle = 0;

    }

    //Dash?
    if(game.input.keyboard.isDown(Phaser.Keyboard.SPACEBAR))
    {
        switch(player.angle)
        {
            case 0:
                player.position.y+= 5;
                break;
            case -90:
                player.position.x+= 5;
                break;
            case -180:
                player.position.y-= 5;
                break;
            case 90:
                player.position.x-= 5;
        }
    }

    player.body.velocity.x = hSpeed * moveSpeed;
    player.body.velocity.y = vSpeed * moveSpeed;

    if(this.client.connected && (player.netId !== null || player.netId !== undefined))
    {
        this.client.sendMessage(player.netId, player.body.position);
    }
}

function Client() {

}

Client.prototype.openConnection = function() {
    this.ws = new WebSocket("ws://174.138.68.104:9001");
    //this.ws = new WebSocket("ws://localhost:9001");
    this.connected = false;
    this.ws.binaryType = "arraybuffer";
    this.ws.onmessage = this.onMessage.bind(this);
    this.ws.onerror = this.displayError.bind(this);
    this.ws.onopen = this.connectionOpen.bind(this);
    this.ws.onclose = this.connectionClose.bind(this);
};

Client.prototype.connectionClose = function() {
    this.connected = false;
};

Client.prototype.sendMessage = function(playerId, payload) {

    var data = new DataView(new ArrayBuffer(13), 0);
    data.setUint8(0, 3, true);    //Set packet id to 3
    data.setUint32(1, playerId, true);
    data.setUint32(5, payload.x, true);
    data.setUint32(9, payload.y, true);
    this.ws.send(data.buffer);
};

Client.prototype.connectionOpen = function() {
    this.connected = true;
};

Client.prototype.onMessage = function(message) {
    if(typeof message.data == 'string') {
        this.processTextMessage(message.data);
    }
    else {
        this.processBinaryMessage(message.data);
    }
};

Client.prototype.processTextMessage = function(msg) {
};

Client.prototype.processBinaryMessage = function(msg) {

    var data = new DataView(msg, 0);
    var packetType = data.getUint8(0, true);

    console.log("packet type: " + packetType);

    if(packetType == 0) {
        //First joined packet
        var assignedId = data.getUint32(1, true);
        var numOtherPlayers = data.getUint32(5, true);
        var otherPlayerId = 0;

        console.log("Assigned Id: " + assignedId);
        console.log("Num Other Players: " + numOtherPlayers);

        player.netId = assignedId;

        for(var i = 0; i < numOtherPlayers; i++) {
            otherPlayerId = data.getUint32(9 + (i*4), true);
            console.log("Other Player Id: " + otherPlayerId);
            otherPlayers[otherPlayerId] = game.add.sprite(0,0,'player');
        }
    }
    else if(packetType == 1) {
        //New player joined packet
        var newPlayerId = data.getUint32(1, true);
        console.log("New Player Id: " + newPlayerId);
        otherPlayers[newPlayerId] = game.add.sprite(0,0,'player');
    }
    else if(packetType == 3) {
        //Player state packet
        var playerId = data.getUint32(1, true);
        var playerX = data.getUint32(5, true);
        var playerY = data.getUint32(9, true);

        otherPlayers[playerId].position.x = playerX;
        otherPlayers[playerId].position.y = playerY;
    }
};

Client.prototype.displayError = function(err) {
    console.log('Websocketerror: ' + err);
};