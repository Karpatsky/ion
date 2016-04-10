#ifndef RPCCONSOLE_H
#define RPCCONSOLE_H

#include "guiutil.h"
#include "net.h"

#include "peertablemodel.h"

#include <QWidget>

namespace Ui {
    class RPCConsole;
}
class ClientModel;

class QItemSelection;
class CNodeCombinedStats;

/** Local Bitcoin RPC console. */
class RPCConsole: public QWidget
{
    Q_OBJECT

public:
    explicit RPCConsole(QWidget *parent = 0);
    ~RPCConsole();

    void setClientModel(ClientModel *model);

    enum MessageClass {
        MC_ERROR,
        MC_DEBUG,
        CMD_REQUEST,
        CMD_REPLY,
        CMD_ERROR
    };

protected:
    virtual bool eventFilter(QObject* obj, QEvent *event);
    void keyPressEvent(QKeyEvent *);

private:
    /** show detailed information on ui about selected node */
    void updateNodeDetail(const CNodeCombinedStats *combinedStats);

    enum ColumnWidths
    {
        ADDRESS_COLUMN_WIDTH = 250,
        MINIMUM_COLUMN_WIDTH = 120
    };

    /** track the node that we are currently viewing detail on in the peers tab */
    CNodeCombinedStats detailNodeStats;

private slots:
    void on_lineEdit_returnPressed();
    void on_tabWidget_currentChanged(int index);
    /** open the debug.log from the current datadir */
    void on_openDebugLogfileButton_clicked();
    /** display messagebox with program parameters (same as bitcoin-qt --help) */
    void on_showCLOptionsButton_clicked();
    /** change the time range of the network traffic graph */
    void on_sldGraphRange_valueChanged(int value);
    /** update traffic statistics */
    void updateTrafficStats(quint64 totalBytesIn, quint64 totalBytesOut);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    /** clear traffic graph */
    void on_btnClearTrafficGraph_clicked();
    /** paste clipboard to line */
    void on_pasteButton_clicked();
    /** copy to clipboard */
    void on_copyButton_clicked();

public slots:
    void clear();
    void message(int category, const QString &message, bool html = false);
    /** Set number of connections shown in the UI */
    void setNumConnections(int count);
    /** Set number of blocks shown in the UI */
    void setNumBlocks(int count);
    /** Set number of masternodes shown in the UI */
    void setMasternodeCount(const QString &strMasternodes);
    /** Go forward or back in history */
    void browseHistory(int offset);
    /** Scroll console view to end */
    void scrollToEnd();
    /** Handle selection of peer in peers list */
    void peerSelected(const QItemSelection &selected, const QItemSelection &deselected);
    /** Handle updated peer information */
    void peerLayoutChanged();
    /** Show folder with wallet backups in default browser */
    void showBackups();
signals:
    // For RPC command executor
    void stopExecutor();
    void cmdRequest(const QString &command);

private:
    static QString FormatBytes(quint64 bytes);
    void setTrafficGraphRange(int mins);

    Ui::RPCConsole *ui;
    ClientModel *clientModel;
    QStringList history;
    GUIUtil::TableViewLastColumnResizingFixer *columnResizingFixer;
    int historyPtr;

    void startExecutor();
};

#endif // RPCCONSOLE_H
