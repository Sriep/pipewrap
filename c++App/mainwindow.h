#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class WorkflowTreeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void Init();

signals:

private slots:
    void newSlot();
    void openSlot();
    void saveSlot();
    void saveAsSlot();
    void openRecentFileSlot();
    void aboutSlot();

private:
    void createActions();
    void createMenus();
    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

QString curFile;
    WorkflowTreeView* m_tree_view;
    QMenu *fileMenu;
    QMenu *recentFilesMenu;
    QMenu *helpMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *separatorAct;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];

};

#endif // MAINWINDOW_H
