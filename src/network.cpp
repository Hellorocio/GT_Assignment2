#include "network.h"

using namespace godot;

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

    register_property<Network, Dictionary>("self_data", &Network::self_data, Dictionary(), GODOT_METHOD_RPC_MODE_DISABLED);
    register_property<Network, Dictionary>("players", &Network::players, Dictionary(), GODOT_METHOD_RPC_MODE_DISABLED);
}

Network::Network() {
    // Network
}

Network::~Network() {

}

void Network::_init() {
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

void Network::create_server(String playerNickname) {
    self_data["name"] = playerNickname;
    self_data["position"] = init_positions[0];
    init_pos_index = 1;
    players[1] = self_data;
    NetworkedMultiplayerENet* peer = NetworkedMultiplayerENet::_new();
    peer->set_bind_ip("*");
    peer->create_server(SERVER_PORT, MAX_PLAYERS);
    get_tree()->set_network_peer(peer);
}

void Network::connect_to_server(String playerNickname, String server_ip) {
    self_data["name"] = playerNickname;
    self_data["position"] = init_positions[init_pos_index];
    init_pos_index++;
    if (init_pos_index = 4)
        init_pos_index = 0;
    get_tree()->connect("connected_to_server", this, "_connected_to_server");
    NetworkedMultiplayerENet* peer = NetworkedMultiplayerENet::_new();
    peer->create_client(server_ip, SERVER_PORT);
    get_tree()->set_network_peer(peer);
}

void Network::_connected_to_server() {
    int64_t localPlayerId = get_tree()->get_network_unique_id();
    players[localPlayerId] = self_data;
    rpc("_send_player_info", localPlayerId, self_data);
}

void Network::_on_player_disconnected(int64_t id) {
    players.erase(id);
}

void Network::_on_player_connected(int64_t connectedPlayerId) {
    Godot::print("Player connected to server");
    int64_t localPlayerId = get_tree()->get_network_unique_id();
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

void Network::_send_player_info(int64_t id, Dictionary info) {
    players[id] = info;

    ResourceLoader *resourceLoader = ResourceLoader::get_singleton();
    player_scene = resourceLoader->load("res://player.tscn");
    BasicMovement *player = Object::cast_to<BasicMovement>(player_scene->instance());
    player->set_name(String(id));
    player->set_network_master(id);
    get_node("/root/Game")->add_child(player);
    player->init(info["name"], info["position"], true);
}

void Network::update_position(int64_t id, Vector3 position) {
    Dictionary player_info = players[id];
    player_info["position"] = position;
}
