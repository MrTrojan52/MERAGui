#include "JSGFParser/include/JSGFParser.h"
#include <QFile>

JSGFParser::JSGFParser(QString filename) {
    m_filename = filename;
}

QStringList JSGFParser::getValuesByVariableName(QString var_name) {
    QFile grammar_file(m_filename);
    QStringList res;
    if(!grammar_file.exists())
        return res;
    if(grammar_file.open(QIODevice::ReadOnly | QIODevice::Text )) {
        while(!grammar_file.atEnd()) {
            QString line = grammar_file.readLine();
            int var_ind = line.indexOf('<' + var_name + '>');
            int eq_ind = line.indexOf('=');
            if(var_ind != -1 && eq_ind != -1 && var_ind < eq_ind) {
                line = line.remove(0, eq_ind + 1);
                line.remove(QChar(';'));
                res = line.split('|');
                for(int i = 0; i < res.size(); ++i)
                {
                    res[i] = res[i].trimmed();
                }
                return res;
            }
        }
    }
    return res;
}
