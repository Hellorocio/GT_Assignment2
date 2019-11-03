
#include "base_ai.h"
#include <queue>
#include <vector>

using namespace godot;

void WanderState::start(Node* parent) {
    Godot::print("start wandering");
    current_waypoint = Object::cast_to<BaseAI>(parent)->_get_closest_node(this);
    path.clear();
}

float randf(float min, float max) {
	return (float) rand() / RAND_MAX * (max - min) + min;
}

void WanderState::execute(Node* parent, float dt) {
    Node* waypoint_parent = parent->get_node("/root/Game/Waypoints");

    BaseAI* parent_ai = Object::cast_to<BaseAI>(parent);

    if (path.empty()) {
        Array children = waypoint_parent->get_children();
        while (true) {
            auto random = children[(int) randf(0, children.size())];
            auto random_spatial = Object::cast_to<Spatial>(Object::___get_from_variant(random));
            if (random_spatial->get_translation().distance_squared_to(parent_ai->get_translation()) > 1) {
                path = parent_ai->get_shortest_path(parent_ai->get_translation(), random_spatial->get_translation());
                break;
            }
        }        
    } else {
        Vector3 target = path.back();
        Vector3 movement = parent_ai->get_movement_vector_to_target(target, dt);
        if (movement == Vector3{}) {
            path.pop_back();
            if (!path.empty()) {
                parent_ai->_turn_to_face((Vector3) path.back());
            }
        } else {
            parent_ai->_update_movement(movement, dt);
        }
    }

    //Godot::print("execute wandering");

    // auto current = parent->get_node("/root/Game/Waypoints/" + current_waypoint);
    // //Godot::print(current_waypoint);

    // if (current != nullptr && !current_waypoint.is_empty()) {
    //     //Godot::print(parent->get_name());
    //     Vector3 delta = Object::cast_to<Spatial>(current)->get_translation() - Object::cast_to<BaseAI>(parent)->get_translation();

    //     if (delta.length() <= 2) {
    //         // find a new waypoint to move to
    //         temp_waypoint = current_waypoint;
    //         current_waypoint = Object::cast_to<BaseAI>(parent)->_get_closest_node(this);
    //         previous_waypoint = temp_waypoint;
    //     } else {
    //         // move ai towards current waypoint
    //         Object::cast_to<BaseAI>(parent)->_update_movement(delta.normalized() * 6, dt);
    //         Object::cast_to<BaseAI>(parent)->_turn_to_face(Object::cast_to<Spatial>(current)->get_translation());
            
    //     }
    // }
}


void WanderState::end(Node* parent) {
    //Godot::print("end wander");
}

void BaseAI::_register_methods() {
	register_method("_process", &BaseAI::_process);
	register_method("_physics_process", &BaseAI::_physics_process);
}

void BaseAI::_process(float delta) {
    brain.update(this, delta);
}

void BaseAI::_physics_process(float delta) {
    move_and_slide(motion, Vector3(0, 1, 0), true, 4, 1.5f);
}


void BaseAI::_update_movement(Vector3 direction, float delta) {
    direction.y = gravity * delta;
    motion = direction;
}

void BaseAI::_turn_to_face(Vector3 target) {
    Vector3 up = Vector3(0, 1, 0);
    target.y = get_translation().y;
    look_at(target, up);
}

Vector3 BaseAI::get_movement_vector_to_target(Vector3 target, float dt) {
    Vector3 delta = target - this->get_translation();
    const float speed = 6;

    float sqr_len = delta.length_squared();
    if (sqr_len <= speed * dt * speed * dt) {
        return Vector3{};
    } else {
        return delta / (sqr_len * sqr_len);
    }
}

NodePath BaseAI::_get_closest_node (WanderState* callingState) {
    //Godot::print("_get_closest_node: start");
    Node* waypoint_parent = get_node("/root/Game/Waypoints");
    
    // With poolStringArray, get neighbors of current_waypoint instead of get_children() if current is not null.
    Array children = waypoint_parent->get_children();
    
    Spatial* child_spatial = Object::cast_to<Spatial>(Object::___get_from_variant(children[0]));
    float min_dist = 1.0/0.0;//(this->get_translation()).distance_squared_to(child_spatial->get_translation());
    int min_index = -1;
    for (int i = 0; i < children.size(); ++i) {
        child_spatial = Object::cast_to<Spatial>(Object::___get_from_variant(children[i]));
        float new_dist = (this->get_translation()).distance_squared_to(child_spatial->get_translation());
        if (child_spatial->get_name() != callingState->previous_waypoint && child_spatial->get_name() != callingState->current_waypoint && new_dist < min_dist) {
            min_dist = new_dist;
            min_index = i;
        }
    }
    if (min_index == -1) {
        return "";
    }
    //Godot::print(Object::cast_to<Node>(Object::___get_from_variant(children[min_index]))->get_name());
    return Object::cast_to<Node>(Object::___get_from_variant(children[min_index]))->get_name();
}

NodePath BaseAI::get_closest_node_to_point(Vector3 pos) {
    Node* waypoint_parent = get_node("/root/Game/Waypoints");
    Array children = waypoint_parent->get_children();
    
    float min_dist = 1.0/0.0;
    int min_index = -1;
    for (int i = 0; i < children.size(); ++i) {
        Spatial* child_spatial = Object::cast_to<Spatial>(Object::___get_from_variant(children[i]));
        float new_dist = pos.distance_squared_to(child_spatial->get_translation());
        if (new_dist < min_dist) {
            min_dist = new_dist;
            min_index = i;
        }
    }
    if (min_index == -1) {
        return "";
    }
    return Object::cast_to<Node>(Object::___get_from_variant(children[min_index]))->get_name();
}

struct NodeDistance {
    float distance;
    Waypoint* waypoint;

    NodeDistance(float distance, Waypoint* waypoint) : distance(distance), waypoint(waypoint) {
    }
};

/**
 * returns a path where path[0] = to and path[len - 1] = from
 */
PoolVector3Array BaseAI::get_shortest_path(Vector3 from, Vector3 to) {
    NodePath from_node = get_closest_node_to_point(from);
    NodePath to_node = get_closest_node_to_point(to);

    Array path = (Array) calculate_shortest_path(from_node, to_node);
    if (path.empty())
        return path;

    path.push_front(to);
    return path;
}

/**
 * returns a path where path[0] = to and path[len - 1] = from
 */
PoolVector3Array BaseAI::calculate_shortest_path(NodePath from, NodePath to) {
    Node* waypoint_parent = get_node("/root/Game/Waypoints");
    auto from_spatial = Object::cast_to<Waypoint>(waypoint_parent->get_node(from));
    auto to_spatial = Object::cast_to<Waypoint>(waypoint_parent->get_node(to));

    if (from == to)
        return Array::make(to_spatial->get_path());

    auto comparer = [](NodeDistance a, NodeDistance b) {
        return a.distance < b.distance;
    };
    std::priority_queue<NodeDistance, std::vector<NodeDistance>, decltype(comparer)> pq(comparer);

    Dictionary distances;
    pq.push(NodeDistance(0, from_spatial));
    distances[from_spatial] = 0;
    
    Dictionary predecessors;

    while (!pq.empty()) {
        Waypoint* u = pq.top().waypoint;
        pq.pop();

        for (int i = 0; i < u->adjacent.size(); ++i) {
            Waypoint* v = Object::cast_to<Waypoint>(waypoint_parent->get_node(String(u->adjacent[i])));
            float weight = u->get_translation().distance_to(v->get_translation());

            float udist = distances[u];
            float vdist = 1.0f / 0.0f;
            if (distances.has(v))
                vdist = distances[v];
            
            if (vdist > udist + weight) {
                vdist = udist + weight;
                pq.push(NodeDistance(vdist, v));
                predecessors[v] = u;
                distances[v] = vdist;
            }
        }
    }

    PoolVector3Array path;
    path.push_back(to_spatial->get_translation());
    auto prev = to_spatial;
    while (true) {
        if (prev->get_path() == from_spatial->get_path())
            break;
        
        if (!predecessors.has(prev))
            return PoolVector3Array{};
        
        Waypoint* pred = predecessors[prev];
        path.push_back(pred->get_translation());

        prev = pred;
    }
    
    return path;
}
