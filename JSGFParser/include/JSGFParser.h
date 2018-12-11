#ifndef JSGFPARSER_H
#define JSGFPARSER_H
#include <QString>
#include <QStringList>
class JSGFParser {
public:
    JSGFParser(QString filename);
    QStringList getValuesByVariableName(QString var_name);

private:
    QString m_filename;
};

#endif
