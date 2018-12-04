#include "IConnectionManager.h"
#include "connectiondata.h"
class AdafruitConnectionManager : public IConnectionManager {
public:
    AdafruitConnectionManager(ConnectionData data) {
        cData = data;
    }
    virtual QString getBaseFeedUrl() override;
    virtual QString getGeneralFeedUrl() override;
    virtual QString getLastValueUrl(ADevice* device) override;
    virtual std::map<QString, std::vector<AvailableDeviceInfo>> getDevicesList() override;
private:
    ConnectionData cData;
};
