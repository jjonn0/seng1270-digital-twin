#include <iostream>
#include <vector>
#include <format>
#include <typeinfo>
#include "profiles.h"

class SimulationEntity
{
    private:
    static int m_total_entities;
    PatientProfile m_patient_profile;
    StaffProfile m_staff_profile;

    public:
    SimulationEntity(PatientProfile patient_profile, StaffProfile staff_profile) : m_patient_profile{patient_profile}, m_staff_profile{staff_profile} {}

    PatientProfile getPatientProfile() { return m_patient_profile; }
    StaffProfile getStaffProfile() { return m_staff_profile; }
    std::string getProfileType()
    {
        if(m_patient_profile.getProfileNumber() != 0 && m_staff_profile.getProfileNumber() == 0)
        {
            return typeid(m_patient_profile).name();
        }
        else if(m_patient_profile.getProfileNumber() == 0 && m_staff_profile.getProfileNumber() != 0)
        {
            return typeid(m_staff_profile).name();
        }
        else
        {
            return "Hybrid";
        }
    }
    std::string toString()
    {
        if(getProfileType() == typeid(m_patient_profile).name())
        {
            return std::format("Associated patient profile: {}", m_patient_profile.toString());
        }
        else if(getProfileType() == typeid(m_staff_profile).name())
        {
            return std::format("Associated staff profile: {}", m_staff_profile.toString());
        }
        else
        {
            return std::format("Associated profiles: {} | {}", m_patient_profile.toString(), m_staff_profile.toString());
        }
    }

    void setPatientProfile(PatientProfile patient_profile) { m_patient_profile = patient_profile; }
    void setStaffProfile(StaffProfile staff_profile) { m_staff_profile = staff_profile; }
};
int SimulationEntity::m_total_entities{0};

void printCurrentEntities(std::vector<SimulationEntity>& entity_list)
{
    for(SimulationEntity& e : entity_list)
    {
        std::cout << e.toString() << std::endl;
    }
}