#pragma once

// system includes
#include <nvs.h>
#include <optional>

class SettingsPersister
{
  public:
    bool init();
    bool erase();
    bool openCommon();
    void closeCommon();
    bool openProfile(uint8_t index);
    void closeProfile();

    template<typename T>
    bool load(T &settings);

    template<typename T>
    bool save(T &settings);

    std::optional<uint8_t> currentlyOpenProfileIndex() const;

  private:
    // for common settings
    nvs_handle m_handle {};

    struct CurrentlyOpenProfile
    {
        nvs_handle handle;
        uint8_t profileIndex;
    };
    std::optional<CurrentlyOpenProfile> m_profile;
};
