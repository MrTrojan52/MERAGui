#include <QString>
#include "Device/include/availabledeviceinfo.h"
#include "Device/include/adevice.h"
class IConnectionManager {
public:
    virtual QString getBaseFeedUrl() = 0;
    virtual QString getGeneralFeedUrl() = 0;
    virtual QString getLastValueUrl(ADevice* device) = 0;
    virtual std::map<QString, std::vector<AvailableDeviceInfo>> getDevicesList() = 0;
    virtual ~IConnectionManager() = default;
};
