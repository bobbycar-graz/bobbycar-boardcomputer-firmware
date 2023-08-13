#pragma once

// system includes
#include <cstdint>
#include <expected>

// 3rdparty lib includes
#include <accessorinterface.h>

// local includes
#include "utils.h"
#include "newsettings.h"

//! Special type of RefAccessor that also saves settings after setValue()
template<typename T>
struct RefAccessorSaveSettings : public virtual espgui::RefAccessor<T>
{
    typename espgui::AccessorInterface<T>::setter_result_t setValue(T value) override
    {
        espgui::RefAccessor<T>::setValue(value);

        if (!saveProfileSettings())
            return tl::make_unexpected("saveProfileSettings() failed!");

        return {};
    };
};

template<typename T>
struct NewSettingsAccessor : public virtual espgui::AccessorInterface<T>
{
    virtual ConfigWrapper<T>& getConfig() const = 0;

    T getValue() const override
    {
        return getConfig().value();
    }

    typename espgui::AccessorInterface<T>::setter_result_t setValue(T value) override
    {
        return configs.write_config(getConfig(), value);
    }
};

template<typename T>
struct NewSettingsChronoAdaptorAccessor;

template<>
struct NewSettingsChronoAdaptorAccessor<espchrono::minutes32> : public virtual espgui::AccessorInterface<int32_t>
{
    virtual ConfigWrapper<espchrono::minutes32>& getConfig() const = 0;

    int32_t getValue() const override
    {
        return getConfig().value().count();
    }

    typename espgui::AccessorInterface<int32_t>::setter_result_t setValue(int32_t value) override
    {
        return configs.write_config(getConfig(), espchrono::minutes32{value});
    }
};

template<>
struct NewSettingsChronoAdaptorAccessor<espchrono::milliseconds32> : public virtual espgui::AccessorInterface<int32_t>
{
    virtual ConfigWrapper<espchrono::milliseconds32>& getConfig() const = 0;

    int32_t getValue() const override
    {
        return getConfig().value().count();
    }

    typename espgui::AccessorInterface<int32_t>::setter_result_t setValue(int32_t value) override
    {
        return configs.write_config(getConfig(), espchrono::milliseconds32{value});
    }
};
