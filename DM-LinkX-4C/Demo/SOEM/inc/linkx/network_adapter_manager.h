#ifndef LINKX_SOEM_DEMO_NETWORK_ADAPTER_MANAGER_H
#define LINKX_SOEM_DEMO_NETWORK_ADAPTER_MANAGER_H

#include "soem.h"

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

struct NetworkAdapterInfo
{
    // SOEM adapter name, for example an NPF device path on Windows. Pass this
    // exact string to ecx_init() when opening the master.
    std::string name;

    // Human-readable NIC description from Npcap/WinPcap.
    std::string description;
};

// Lightweight RAII helper for SOEM network adapters. It owns no SOEM context;
// it only scans adapters, remembers the selected index, and closes the master
// if this object opened it.
class NetworkAdapterManager
{
public:
    explicit NetworkAdapterManager(ecx_contextt& context);
    ~NetworkAdapterManager();

    NetworkAdapterManager(const NetworkAdapterManager&) = delete;
    NetworkAdapterManager& operator=(const NetworkAdapterManager&) = delete;

    // Refresh the adapter list. Any opened adapter is closed before rescanning.
    bool scan();
    void show_adapters(std::ostream& output) const;

    // Select/open by zero-based index from adapters().
    bool select_adapter(std::size_t index);
    bool open_selected_adapter();
    bool open_adapter(std::size_t index);
    void close();

    [[nodiscard]] bool has_selection() const;
    [[nodiscard]] bool is_open() const;
    [[nodiscard]] std::size_t selected_index() const;
    [[nodiscard]] const std::string& selected_name() const;
    [[nodiscard]] const std::vector<NetworkAdapterInfo>& adapters() const;

private:
    static constexpr std::size_t invalid_index = static_cast<std::size_t>(-1);

    ecx_contextt& context_;
    std::vector<NetworkAdapterInfo> adapters_;
    std::size_t selected_index_ = invalid_index;
    bool opened_ = false;
};

#endif // LINKX_SOEM_DEMO_NETWORK_ADAPTER_MANAGER_H
