#pragma once
#include <iostream>
#include <string>
#include <format>
#include <vector>
#include "profiles.h"

class Room
{
    private:
    size_t m_room_id;         // Unique room id
    std::string m_room_name;  // Name displayed for the room

    public:
    explicit Room(size_t room_id) : m_room_id{room_id} {}
    Room(size_t room_id, std::string room_name) : m_room_id{room_id}, m_room_name{room_name} {}

    size_t getRoomId() { return m_room_id; }
    std::string getRoomName() { return m_room_name; }
    std::string toString() { return std::format("{} {}", m_room_id, m_room_name); }

    void setRoomName(std::string room_name) { m_room_name = room_name; }
};

class PatientRoom : public Room
{
    private:
    std::vector<PatientProfile> t;
    
    public:
    PatientRoom(size_t room_id) : Room(room_id) {}
};