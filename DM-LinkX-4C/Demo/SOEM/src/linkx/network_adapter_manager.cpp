#include "network_adapter_manager.h"

#include <iostream>

NetworkAdapterManager::NetworkAdapterManager(ecx_contextt& context)
    : context_(context)
{
}

NetworkAdapterManager::~NetworkAdapterManager()
{
    close();
}

bool NetworkAdapterManager::scan()
{
    close();
    adapters_.clear();
    selected_index_ = invalid_index;

    // ec_find_adapters() allocates a linked list owned by SOEM. Copy the fields
    // we need, then release the list before returning to the caller.
    ec_adaptert* const head = ec_find_adapters();
    for (ec_adaptert* adapter = head; adapter != nullptr; adapter = adapter->next)
    {
        adapters_.push_back({
            adapter->name != nullptr ? adapter->name : "",
            adapter->desc != nullptr ? adapter->desc : "",
        });
    }
    if (head != nullptr)
    {
        ec_free_adapters(head);
    }

    return !adapters_.empty();
}

void NetworkAdapterManager::show_adapters(std::ostream& output) const
{
    if (adapters_.empty())
    {
        output << "No adapters found.\n";
        return;
    }

    output << "Available adapters:\n";
    for (std::size_t i = 0; i < adapters_.size(); ++i)
    {
        output << "   " << i << " - " << adapters_[i].name
               << "  (" << adapters_[i].description << ")\n";
    }
}

bool NetworkAdapterManager::select_adapter(std::size_t index)
{
    if (index >= adapters_.size())
    {
        selected_index_ = invalid_index;
        return false;
    }

    selected_index_ = index;
    return true;
}

bool NetworkAdapterManager::open_selected_adapter()
{
    if (!has_selection())
    {
        return false;
    }

    close();
    // The selected adapter name must be SOEM's raw name, not the human-readable
    // description. On Windows this is usually an NPF device path from Npcap.
    opened_ = ecx_init(&context_, adapters_[selected_index_].name.c_str()) != 0;
    return opened_;
}

bool NetworkAdapterManager::open_adapter(std::size_t index)
{
    return select_adapter(index) && open_selected_adapter();
}

void NetworkAdapterManager::close()
{
    if (opened_)
    {
        ecx_close(&context_);
        opened_ = false;
    }
}

bool NetworkAdapterManager::has_selection() const
{
    return selected_index_ < adapters_.size();
}

bool NetworkAdapterManager::is_open() const
{
    return opened_;
}

std::size_t NetworkAdapterManager::selected_index() const
{
    return selected_index_;
}

const std::string& NetworkAdapterManager::selected_name() const
{
    static const std::string empty_name;
    return has_selection() ? adapters_[selected_index_].name : empty_name;
}

const std::vector<NetworkAdapterInfo>& NetworkAdapterManager::adapters() const
{
    return adapters_;
}
