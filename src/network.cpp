#include "network.h"

using namespace godot;

// This code was referenced from https://github.com/devonh/Godot-engine-tutorial-demos/tree/master/2018/07-30-2018-multiplayer-high-level-api
void Network::_register_methods() {
    register_method("_init", &Network::_init, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_ready", &Network::_ready, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("create_server", &Network::create_server, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("connect_to_server", &Network::connect_to_server, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_connected_to_server", &Network::_connected_to_server, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_player_disconnected", &Network::_on_player_disconnected, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_on_player_connected", &Network::_on_player_connected, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("_request_player_info", &Network::_request_player_info, GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("_request_players", &Network::_request_players, GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("_send_player_info", &Network::_send_player_info, GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("update_position", &Network::update_position, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("set_play_pressed", &Network::set_play_pressed, GODOT_METHOD_RPC_MODE_DISABLED);
    register_method("update_play_pressed", &Network::update_play_pressed, GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("start_game", &Network::start_game, GODOT_METHOD_RPC_MODE_REMOTESYNC);


    register_property<Network, Dictionary>("self_data", &Network::self_data, Dictionary(), GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Network, Dictionary>("players", &Network::players, Dictionary(), GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Network, bool>("is_started", &Network::is_started, false, GODOT_METHOD_RPC_MODE_DISABLED);
}

Network::Network() {
    // Network
}

Network::~Network() {

}

void Network::_init() {
    is_started = false;
    self_data["name"] = "";
    self_data["position"] = Vector3(0, 2, 0);
}

void Network::_ready() {
    get_tree()->connect("network_peer_disconnected", this, "_on_player_disconnected");
    get_tree()->connect("network_peer_connected", this, "_on_player_connected");
    init_positions.resize(4);
    init_positions[0] = Vector3(0,2,0);
    init_positions[1] = Vector3(47,28,-31);
    init_positions[2] = Vector3(-18,14,39);
    init_positions[3] = Vector3(-42,1,-21);
}

// called by 1, the server
void Network::create_server(String playerNickname) {
    //set up self_data dictionary with important player info that will be shared across server and clients
    self_data["name"] = playerNickname;
    self_data["position"] = init_positions[0];
    self_data["acorns_collected"] = 0;
    self_data["play_pressed"] = false;
    
    //init server
    init_pos_index = 1;
    players[1] = self_data;
    NetworkedMultiplayerENet* peer = NetworkedMultiplayerENet::_new();
    peer->set_bind_ip("*");
    peer->create_server(SERVER_PORT, MAX_PLAYERS);
    get_tree()->set_network_peer(peer);
}

// called by each client 
void Network::connect_to_server(String playerNickname, String server_ip) {
    //set up self_data dictionary with important player info that will be shared across server and clients
    self_data["name"] = playerNickname;
    self_data["position"] = init_positions[init_pos_index];
    self_data["acorns_collected"] = 0;
    self_data["play_pressed"] = false;

    //init client
    init_pos_index++;
    if (init_pos_index = 4)
        init_pos_index = 0;
    get_tree()->connect("connected_to_server", this, "_connected_to_server");
    NetworkedMultiplayerENet* peer = NetworkedMultiplayerENet::_new();
    peer->create_client(server_ip, SERVER_PORT);
    get_tree()->set_network_peer(peer);
}

// Called by new clients when they connect to the server
void Network::_connected_to_server() {
    int64_t localPlayerId = get_tree()->get_network_unique_id();
    players[localPlayerId] = self_data;
    rpc("_send_player_info", localPlayerId, self_data);
}

void Network::_on_player_disconnected(int64_t id) {
    players.erase(id);

    auto disconnected_player = get_node("/root/Game/" + String::num_int64(id));
    disconnected_player->queue_free();
}

void Network::_on_player_connected(int64_t connectedPlayerId) {
    int64_t localPlayerId = get_tree()->get_network_unique_id();
    Godot::print("_on_player_connected: Player connected to server " + String::num_int64(localPlayerId) + " " + String::num_int64(connectedPlayerId));
    if(!get_tree()->is_network_server()) {
        rpc_id(1, "_request_player_info", localPlayerId, connectedPlayerId);
    }
}

void Network::_request_player_info(int64_t requestFromId, int64_t playerId) {
    if(get_tree()->is_network_server()) {
        rpc_id(requestFromId, "_send_player_info", playerId, players[playerId]);
    }
}

void Network::_request_players(int64_t requestFromId) {
    if(get_tree()->is_network_server()) {
        for(int64_t peerId = 0; peerId < players.size(); peerId++) {
            if(peerId != requestFromId) {
                rpc_id(requestFromId, "_send_player_info", peerId, players[peerId]);
            }
        }
    }
}

// This is sent as an RPC to all clients and servers when a client joins
void Network::_send_player_info(int64_t id, Dictionary info) {
    int64_t localPlayerId = get_tree()->get_network_unique_id();
    Godot::print("send_player_info: Player connected to server " + String::num_int64(localPlayerId) + " " + String::num_int64(id));

    players[id] = info;

    if (get_tree()->is_network_server()) {
        rset_id(localPlayerId, "is_started", is_started);
    }

    //update lobby if we're in the lobby
    Label* label = Object::cast_to<Label>(get_node("/root/Game/LobbyMenu/PlayerList"));
    label->set_text(label->get_text()+info["name"]+"\n");

    //add player to the scene
    ResourceLoader *resourceLoader = ResourceLoader::get_singleton();
    player_scene = resourceLoader->load("res://player.tscn");
    BasicMovement *player = Object::cast_to<BasicMovement>(player_scene->instance());
    player->set_name(String::num_int64(id));
    player->set_network_master(id);
    get_node("/root/Game")->add_child(player);
    Godot::print(String::num_int64(players.size()));
    player->init(info["name"], info["position"], true);    
}

void Network::update_position(int64_t id, Vector3 position) {
    Dictionary player_info = players[id];
    player_info["position"] = position;
}

// This is called by the client when they press play in the lobby
void Network::set_play_pressed() {
    self_data["play_pressed"] = true;

    if (is_started) {
        start_game();
        return;
    }

    if (!get_tree()->is_network_server())
        rpc_id(1, "update_play_pressed", get_tree()->get_network_unique_id());    
    else
        update_play_pressed(1);      
}

// This is sent as an RPC to the server to update the play_pressed key in the player dictionary with the given id
// The server then checks if all players have pressed play, and starts the game if they have
void Network::update_play_pressed(int64_t id) {
    String id_string = String::num_int64(id);
    Godot::print(id_string);
    Dictionary player_info = players[id];    
    player_info["play_pressed"] = true;
    
    //check if all players and the have pressed the play button
    Array keys = players.keys();
    bool start_game = true;
    for (int i = 0; i < keys.size(); ++i) {
        Godot::print("update_play_pressed: checking player dict for play_pressed val");
        Dictionary player_data = players[keys[i]];

        if (!static_cast<bool>(player_data["play_pressed"])) {
            Godot::print("player play_pressed is false, don't start game");
            start_game = false;
            break;
        }
    }

    if (start_game) {
        if (!is_started) {
            Timer* timer = Object::cast_to<Timer>(get_parent()->get_node("/root/Game/GUI/Timer"));
            timer->start(120);
        }
        rpc("start_game");
    }
}

void Network::start_game() {    
    is_started = true;
	Control* lobby_menu = Object::cast_to<Control>(get_parent()->get_node("/root/Game/LobbyMenu"));
    if (lobby_menu) {
		lobby_menu->hide();
		get_tree()->set_pause(false);
    }
}