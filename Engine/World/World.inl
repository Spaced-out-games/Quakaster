
namespace qk {

    template <class... ID_Ts>
    World<ID_Ts...>::~World() = default;

    // CRUD

    template <class... ID_Ts>
    std::expected<qk::HObject<ID_Ts...>, VDriveError>
        World<ID_Ts...>::create(const KeyValues&) {
        return std::unexpected (std::nullopt);
    }

    template <class... ID_Ts>
    std::expected<KeyValues, VDriveError>
        World<ID_Ts...>::read(qk::HObject<ID_Ts...>, const KeyValues&) const {
        return std::unexpected (std::nullopt);
    }

    template <class... ID_Ts>
    std::expected<void, VDriveError>
        World<ID_Ts...>::update(qk::HObject<ID_Ts...>, const KeyValues&) {
        return std::unexpected (std::nullopt);
    }

    template <class... ID_Ts>
    std::expected<bool, VDriveError>
        World<ID_Ts...>::destroy(qk::HObject<ID_Ts...>, const KeyValues&) {
        return std::unexpected (std::nullopt);
    }

    // Validation

    template <class... ID_Ts>
    std::expected<bool, VDriveError>
        World<ID_Ts...>::valid(qk::HObject<ID_Ts...>) const {
        return std::unexpected (std::nullopt);
    }

    // Lookup

    template <class... ID_Ts>
    std::expected<qk::HObject<ID_Ts...>, VDriveError>
        World<ID_Ts...>::lookup(str_view) const {
        return std::unexpected (std::nullopt);
    }

    template <class... ID_Ts>
    std::expected<std::vector<qk::HObject<ID_Ts...>>, VDriveError>
        World<ID_Ts...>::find(const Query&) const {
        return std::unexpected (std::nullopt);
    }

    template <class... ID_Ts>
    str World<ID_Ts...>::to_string(
        qk::HObject<ID_Ts...>,
        EntryDisplayFlags) {
        return "";
    }

    template <class... ID_Ts>
    void World<ID_Ts...>::list(
        qk::HObject<ID_Ts...>,
        const std::vector<str>&,
        EntryDisplayFlags) const {
    }

}