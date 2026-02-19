#pragma once
#include <iostream>
#include <vector>
#include <format>
#include <typeinfo>
#include "profiles.h"

class SimulationEntity
{
    private:
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
    std::string toFormattedString()
    {
        if(getProfileType() == typeid(m_patient_profile).name())
        {
            return std::format("Found Patient Record:\n{}", m_patient_profile.toFormattedString());
        }
        else if(getProfileType() == typeid(m_staff_profile).name())
        {
            return std::format("Found Staff Record:\n{}", m_staff_profile.toFormattedString());
        }
        else
        {
            return std::format("Found Patient and Staff Record:\n{}\n{}", m_patient_profile.toFormattedString(), m_staff_profile.toFormattedString());
        }
    }

    void setPatientProfile(PatientProfile patient_profile) { m_patient_profile = patient_profile; }
    void setStaffProfile(StaffProfile staff_profile) { m_staff_profile = staff_profile; }
};