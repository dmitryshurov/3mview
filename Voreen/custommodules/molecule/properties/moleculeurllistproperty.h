#ifndef VRN_MOLECULEURLLISTPROPERTY_H
#define VRN_MOLECULEURLLISTPROPERTY_H

#include "voreen/core/properties/templateproperty.h"
#include "voreen/core/properties/condition.h"

#include <vector>
#include <map>

namespace voreen {

class MoleculeCollection;
class Molecule;
class ProgressBar;

#ifdef DLL_TEMPLATE_INST
template class VRN_CORE_API TemplateProperty<std::vector<std::string> >;
#endif

/**
 * Property that stores a list of volume source URLs.
 *
 * Additionally, the property provides functionality to load volumes from the assigned URLs
 * and to maintain their selection state.
 */
class VRN_CORE_API MoleculeURLListProperty : public TemplateProperty<std::vector<std::string> > {
public:
    /**
     * Constructor.
     *
     * @param ident identifier that is used in serialization
     * @param guiText text that is shown in the gui
     * @param value the initial source URLs to assign
     */
    MoleculeURLListProperty(const std::string& id, const std::string& guiText, const std::vector<std::string>& value, bool isLoadable = false,
        int invalidationLevel = Processor::INVALID_PARAMETERS);
    MoleculeURLListProperty();

    virtual Property* create() const;

    virtual std::string getClassName() const       { return "MoleculeURLListProperty"; }
    virtual std::string getTypeDescription() const { return "URL List"; }

    /**
     * Assigns the passed URL list to the property, and clears the currently loaded volumes.
     * All passed URLs are marked as not selected. No volumes are loaded.
     *
     * @see loadMolecules
     */
    void set(const std::vector<std::string>& URLs);

    /**
     * Assigns the passed URL list to the property, and clears the currently loaded volumes.
     * No volumes are loaded.
     *
     * @param URLs the URLs to assign to the property
     * @param selected if true, the assigned URLs are marked as selected
     *
     * @see loadMolecules
     */
    void setURLs(const std::vector<std::string>& URLs, bool selected = false);

    /**
     * Adds the passed URL, if not already contained by the property.
     *
     * @param url the URL to add to the property
     * @param selected if true, the added URL is marked as selected
     */
    void addURL(const std::string& url, bool selected = false);

    /**
     * Removes the passed URL and deletes the corresponding volume handle,
     * if the handle is owned by the property.
     */
    void removeURL(const std::string& url);

    /**
     * Returns the stored list of source URLs.
     */
    const std::vector<std::string>& getURLs() const;

    /**
     * Returns whether the property contains the passed URL.
     */
    bool containsURL(const std::string& url) const;

    /**
     * Adds the passed handle's URL to the property and stores the handle.
     * If the handle's URL is already registered and has a volume handle assigned,
     * the volume handle is replaced by the passed one.
     *
     * @param handle the volume handle to add to the property
     * @param owner if true, the property takes ownership of the volume handle
     * @param selected if true, the handle's URL is marked as selected
     */
    void addMolecule(Molecule* handle, bool owner = false, bool selected = false);

    /**
     * Removes the passed volume handle and deletes it, if it is owned by the property.
     * The volume's URL is not removed from the property.
     *
     * @see removeURL
     */
    void removeMolecule(Molecule* handle);

    /**
     * Returns a collection containing the volumes that have
     * been loaded from the assigned source URLs.
     *
     * @param selectedOnly if true, only the selected volumes are returned
     * @return a new MoleculeCollection, deleting it is up to the caller
     */
    MoleculeCollection* getMolecules(bool selectedOnly = false) const;

    /**
     * Returns the volume corresponding to the passed URL,
     * or null if the volume has not been loaded, yet.
     */
    Molecule* getMolecule(const std::string& url) const;

    /// Sets the selection state of the passed URL.
    void setSelected(const std::string& url, bool selected);

    /// Sets the selection state of all assigned URLs.
    void setAllSelected(bool selected);

    /// Returns whether the passed URL is selected.
    bool isSelected(const std::string& url) const;

    /**
     * Loads the volume from passed source URL.
     * If the volume has already been loaded, it is reloaded.
     *
     * @note The passed URL must be contained by the property.
     *
     * @note The property takes ownership of the loaded volume handle
     *       and deletes it when the URL is removed from the property
     *       or the property is destructed.
     *
     * @throws FileException, bad_alloc if the volume could not be loaded
     */
    void loadMolecule(const std::string& url)
        throw (tgt::FileException, std::bad_alloc);

    /**
     * Loads volumes from the currently assigned source URLs. Already
     * loaded volumes are ignored.
     *
     * @param selectedOnly if true, only the selected volumes are loaded
     * @param removeOnFailure if true, URLs of volumes that could not be loaded are removed
     *
     * @note The property takes ownership of the loaded
     *       volume handles and deletes them on its own
     *       destruction or when the corresponding URL is removed.
     */
    void loadMolecules(bool selectedOnly = false, bool removeOnFailure = false);

    /**
     * Clears the URL list and deletes all volumes that are owned
     * by the property.
     */
    void clear();
    
    bool isLoadable() const { return isLoadable_; }

    /// @see Property::serialize
    virtual void serialize(XmlSerializer& s) const;

    /// @see Property::deserialize
    virtual void deserialize(XmlDeserializer& s);

protected:
    /// Clears the property, thereby deleting all volume that are owned by it.
    virtual void deinitialize() throw (tgt::Exception);

private:
    /// Returns whether the volume corresponding to the passed URL is owned by the property.
    bool isOwner(const std::string& url) const;

    /// Returns the property's progress bar and generates it on first access.
    ProgressBar* getProgressBar();

    std::map<std::string, Molecule*> handleMap_; ///< maps from URL to molecule handle (transient)
    std::map<std::string, bool> selectionMap_; ///< maps from URL to selection state (persisted)
    std::map<std::string, bool> ownerMap_;     ///< maps from URL to owner state

    ProgressBar* progressBar_;
    bool isLoadable_; ///< Flag that switches loading of the molecules

    static const std::string loggerCat_;
};

} // namespace voreen

#endif // VRN_VOLUMEURLLISTPROPERTY_H
