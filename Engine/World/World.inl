
namespace qk {

    
    World::~World() = default;


    
    void World::execute_loader(qk::UURH target, const KeyValues& args) {
        m_Loader.execute(m_Registry, target.to<entt::entity>(), args);
    }


    // CRUD

    
    std::expected<qk::UURH, VDriveError>
        World::create(const KeyValues&) {
        return std::unexpected (std::nullopt);
    }

    
    std::expected<KeyValues, VDriveError>
        World::read(qk::UURH, const KeyValues&) const {
        return std::unexpected (std::nullopt);
    }

    
    std::expected<void, VDriveError>
        World::update(qk::UURH, const KeyValues&) {
        return std::unexpected (std::nullopt);
    }

    
    std::expected<bool, VDriveError>
        World::destroy(qk::UURH, const KeyValues&) {
        return std::unexpected (std::nullopt);
    }

    // Validation

    
    std::expected<bool, VDriveError>
        World::valid(qk::UURH) const {
        return std::unexpected (std::nullopt);
    }

    // Lookup

    
    std::expected<qk::UURH, VDriveError>
        World::lookup(str_view) const {
        return std::unexpected (std::nullopt);
    }

    
    std::expected<std::vector<qk::UURH>, VDriveError>
        World::find(const Query&) const {
        return std::unexpected (std::nullopt);
    }

    
    str World::to_string(
        qk::UURH,
        EntryDisplayFlags) {
        return "";
    }

    
    void World::list(
        qk::UURH,
        const std::vector<str>&,
        EntryDisplayFlags) const {
    }

}