    /*!
	  * http://qt-project.org/wiki/DatabaseConnectionDialog
      * \file databasedialog.h
      */
    #ifndef DATABASEDIALOG_H
    #define DATABASEDIALOG_H
     
    #include <QDialog>
    #include <QLabel>
    #include <QLineEdit>
    #include <QComboBox>
    #include <QSpinBox>
    #include <QDialogButtonBox>
    #include <QHBoxLayout>
    #include <QVBoxLayout>
    #include <QGridLayout>
    #include <QSqlDatabase>
    #include <QString>
    #include <QMessageBox>
    #include <QDebug>
    #include <QSqlError>
    #include <QPushButton>
    #include <QGroupBox>
     
    class DatabaseConnectionDialog : public QDialog
    {
        Q_OBJECT
     
    private:
     
        /*!
          * The display label for the database driver name.
          */
        QLabel* labelDatabaseDriverName;
     
        /*!
          * The display label for the TCP/IP port the database
          * is listening for connections.
          */
        QLabel* labelDatabasePort;
     
        /*!
          * The label for the database name.
          */
        QLabel* labelDatabaseName;
     
        /*!
          * The label for the database host name.
          */
        QLabel* labelDatabaseHostName;
     
        /*!
          * The label for the database username.
          */
        QLabel* labelDatabaseUsername;
     
        /*!
          * The label for the database password.
          */
        QLabel* labelDatabasePassword;
     
        /*!
          * A label to display the summary database URL
          * connection string.
          */
        QLabel* labelDatabaseURL;
     
     
        /*!
          * The editable name of the database to which the user
          * wants to connect to.
          */
        QLineEdit* editDatabaseName;
     
        /*!
          * The editable name of the database name to which connect.
          */
        QLineEdit* editDatabaseHostName;
     
        /*!
          * The database listening port.
          */
        QSpinBox* spinBoxDatabasePort;
     
        /*!
          * The editable username to use for the connection.
          */
        QLineEdit* editDatabaseUsername;
     
        /*!
          * The editable password to use for the connection.
          */
        QLineEdit* editDatabasePassword;
     
        /*!
          * The combo from which the user can select the database
          * driver name and type
          */
        QComboBox* comboDatabaseDriverName;
     
     
        /*!
          * A Dialog button box for displaying the
          * connect/cancel buttons.
          */
        QDialogButtonBox* buttons;
     
        /*!
          * A method to create all the components of this dialog window
          * and lay out them correctly.
          */
        void setUpGUI();
     
        /*!
          * Searches for and populates the combo box with the
          * available database drivers.
          */
        void findAvailableDrivers();
     
     
        /*!
          * Performs the connection to the database
          * and emits the signal to pass the connection.
          */
        void doDatabaseConnection();
     
    public:
        explicit DatabaseConnectionDialog(QWidget *parent = 0);
     
        /*!
          * Sets the database name in the dialog.
          * \param dbName the name of the database
          */
        void setDatabaseName( const QString& dbName );
     
        /*!
          * Sets the port number for the database connection.
          * \param portNumber the port number the database is listening for
          * connections
          */
        void setDatabasePortNumber( int& portNumber );
     
        /*!
          * Sets the remote host name mnemonic name.
          * \param hostname the name of the host the database is running on
          */
        void setDatabaseHostName( const QString& hostname );
     
        /*!
          * Sets the username to use for the connection.
          * \param username the username to use for the connection
          */
        void setDatabaseUsername( const QString& username );
     
        /*!
          * Selects the driver name.
          * \param the driver name (therefore the database type) to
          * select in the combo box.
          */
        void setDatabaseDriverName( const QString& drvName );
     
        /*!
          * Sets the user password.
          * \param the password to use for the connection
          */
        void setDatabasePassword( const QString& pwd );
     
     
        /*!
          * Performs a check against the user data and enables/disables
          * the connect button depending on the form fill status.
          *\return true if the data allows a database connection
          */
        bool checkFormData();
     
        /*!
          * Performs the database connection or prompt the user
          * showing this dialog in the case data is not completed
          * or should not perform the autoconnection.
          * \param autoConnect if set to true tries to perform an automatic
          * connection to the database, if the data is complete, or prompt the user
          * for missing data. If set to false, simply shows the dialog and waits.
          */
        void run( bool autoConnect );
     
     
    signals:
     
        /*!
          * Passes the database connection in the case the connection
          * is succesful.
          * \param databaseConnection the connection object
          */
        void databaseConnect( QSqlDatabase& databaseConnection );
     
    public slots:
     
        /*!
          * Checks if the user has entered enough data to
          * try a database connection.
          */
        bool slotCheckFormData();
     
     
        /*!
          * Performs the database connection.
          */
        void slotPerformConnection();
     
    };
     
    #endif // DATABASEDIALOG_H
