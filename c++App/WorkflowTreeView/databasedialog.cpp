#include "databasedialog.h"
// http://qt-project.org/wiki/DatabaseConnectionDialog
DatabaseConnectionDialog::DatabaseConnectionDialog(QWidget *parent) :
    QDialog(parent)
{
    // this dialog is modal
    setModal( true );
    // the title of this dialog
    setWindowTitle( tr("Database connection") );
    // place each GUI component
    setUpGUI();
    // load available drivers
    findAvailableDrivers();
}

void DatabaseConnectionDialog::setUpGUI()
{


    // create all gui components
    labelDatabaseDriverName = new QLabel( tr("Database Type (driver name)"), this );
    labelDatabasePort       = new QLabel( tr("TCP/IP Port Number"), this );
    labelDatabaseName       = new QLabel( tr("Database Name"), this );
    labelDatabaseHostName   = new QLabel( tr("Host Name"), this );
    labelDatabaseUsername   = new QLabel( tr("Username"), this );
    labelDatabasePassword   = new QLabel( tr("Password"), this );
    labelDatabaseURL        = new QLabel( this );
    labelDatabaseURL->setAlignment( Qt::AlignCenter );

    spinBoxDatabasePort     = new QSpinBox( this );
    spinBoxDatabasePort->setMaximum( 9999 );
    spinBoxDatabasePort->setMinimum( 100 );
    spinBoxDatabasePort->setSingleStep( 1 );

    comboDatabaseDriverName = new QComboBox( this );
    comboDatabaseDriverName->setEditable( false );


    editDatabaseName = new QLineEdit( this );
    editDatabaseHostName = new QLineEdit( this );
    editDatabaseUsername = new QLineEdit( this );
    editDatabasePassword = new QLineEdit( this );
    editDatabasePassword->setEchoMode( QLineEdit::Password );
    connect( editDatabaseName,
             SIGNAL(editingFinished()),
             this,
             SLOT(slotCheckFormData()) );
    connect( editDatabaseHostName,
             SIGNAL(editingFinished()),
             this,
             SLOT(slotCheckFormData()) );
    connect( editDatabaseUsername,
             SIGNAL(editingFinished()),
             this,
             SLOT(slotCheckFormData()) );
    connect( editDatabasePassword,
             SIGNAL(editingFinished()),
             this,
             SLOT(slotCheckFormData()) );
    connect( editDatabasePassword,
             SIGNAL(returnPressed()),
             this,
             SLOT(slotCheckFormData()) );


    // create the button box
    buttons = new QDialogButtonBox( this );
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton( QDialogButtonBox::Cancel );
    QPushButton* okButton = buttons->button( QDialogButtonBox::Ok );
    okButton->setText( tr( "Connect!" ) );
    okButton->setEnabled( false );
    connect( buttons,
             SIGNAL(accepted()),
             this,
             SLOT(slotPerformConnection()));
    connect( buttons,
             SIGNAL(rejected()),
             this,
             SLOT(close()));





    // create a vertical layout to display components
    QVBoxLayout* verticalLayout = new QVBoxLayout( this );


    // create a grid layout to add all the components
    QGridLayout* formGridLayout = new QGridLayout( this );
    QGroupBox* gridGroupBox = new QGroupBox( this );
    gridGroupBox->setTitle( tr("Database connection properties" ) );
    formGridLayout->addWidget( labelDatabaseDriverName, 0, 0 );
    formGridLayout->addWidget( comboDatabaseDriverName, 0, 1 );
    labelDatabaseDriverName->setBuddy( comboDatabaseDriverName );
    formGridLayout->addWidget( labelDatabaseHostName, 1, 0 );
    formGridLayout->addWidget( editDatabaseHostName, 1, 1);
    labelDatabaseHostName->setBuddy( editDatabaseHostName );
    formGridLayout->addWidget( labelDatabasePort, 2, 0 );
    formGridLayout->addWidget( spinBoxDatabasePort, 2, 1 );
    labelDatabasePort->setBuddy( spinBoxDatabasePort );
    formGridLayout->addWidget( labelDatabaseName, 3, 0 );
    formGridLayout->addWidget( editDatabaseName , 3, 1 );
    labelDatabaseName->setBuddy( editDatabaseName );
    formGridLayout->addWidget( labelDatabaseUsername, 4, 0 );
    formGridLayout->addWidget( editDatabaseUsername, 4, 1 );
    labelDatabaseUsername->setBuddy( editDatabaseUsername );
    formGridLayout->addWidget( labelDatabasePassword, 5, 0 );
    formGridLayout->addWidget( editDatabasePassword, 5, 1 );
    labelDatabasePassword->setBuddy( editDatabasePassword );
    // add all the elements to groupbox
    gridGroupBox->setLayout( formGridLayout );

    // place a new groupbox to contain the database connection URL
    QGroupBox* urlGroupBox = new QGroupBox( this );
    urlGroupBox->setTitle( tr( "Database URL" ) );
    QHBoxLayout* urlLayout = new QHBoxLayout( this );
    urlLayout->addWidget( labelDatabaseURL );
    urlGroupBox->setLayout( urlLayout );



    // nest all layouts together
    verticalLayout->addWidget( gridGroupBox );
    verticalLayout->addStretch();
    verticalLayout->addWidget( urlGroupBox );
    verticalLayout->addWidget( buttons );


    comboDatabaseDriverName->setFocus();
}

void DatabaseConnectionDialog::findAvailableDrivers()
{
    // remove all items
    comboDatabaseDriverName->clear();

    // populate the combo box with all available drivers
    foreach( QString driverName, QSqlDatabase::drivers() )
        comboDatabaseDriverName->addItem( driverName );
}

bool DatabaseConnectionDialog::slotCheckFormData()
{
    return checkFormData();
}


bool DatabaseConnectionDialog::checkFormData(){
    if( editDatabaseName->text().isEmpty()
       || editDatabaseHostName->text().isEmpty())
       //|| editDatabaseUsername->text().isEmpty()
       //|| editDatabasePassword->text().isEmpty() )
        buttons->button( QDialogButtonBox::Ok )->setEnabled( false );
    else{
        // enable the connect button and give focus
        buttons->button( QDialogButtonBox::Ok )->setEnabled( true );
        buttons->button( QDialogButtonBox::Ok )->setFocus();
    }


    // if the connection can be established (or at least tried)
    // display the URL
    if( buttons->button( QDialogButtonBox::Ok )->isEnabled() )
        labelDatabaseURL->setText( comboDatabaseDriverName->currentText()
                                   + "://"
                                   + editDatabaseUsername->text()
                                   + "@"
                                   + editDatabaseHostName->text()
                                   + "/"
                                   + editDatabaseName->text() );
    else
        labelDatabaseURL->setText( "" );


    return buttons->button( QDialogButtonBox::Ok )->isEnabled();
}

void DatabaseConnectionDialog::doDatabaseConnection()
{
    // check the database driver is really available
    // (should never happen)
    if( ! QSqlDatabase::isDriverAvailable( comboDatabaseDriverName->currentText() ) ){
        QMessageBox::critical( this,
                               tr("Database Connection Error"),
                               tr("Database driver not available!")
                               );


        return;
    }

    qDebug() << "Performing the database driver setup..";

    // set database driver properties
    QSqlDatabase databaseConnection = QSqlDatabase::addDatabase( comboDatabaseDriverName->currentText() );
    databaseConnection.setDatabaseName( editDatabaseName->text() );
    databaseConnection.setUserName( editDatabaseUsername->text() );
    databaseConnection.setHostName( editDatabaseHostName->text() );
    databaseConnection.setPassword( editDatabasePassword->text() );
    databaseConnection.setPort( spinBoxDatabasePort->text().toInt() );

    if( ! databaseConnection.open() ){
        QMessageBox::critical( this,
                               tr("Database Connection Error"),
                               databaseConnection.lastError().text()
                               );

        // disable the connect button and set the focus again
        // on the first field
        buttons->button( QDialogButtonBox::Ok )->setEnabled( false );
        editDatabaseHostName->setFocus();
    }
    else{
        // hide the dialog
        this->hide();

        // emit the signal
        qDebug() << "Emitting signal since the database is connected!";
        emit databaseConnect( databaseConnection );
    }
}

void DatabaseConnectionDialog::slotPerformConnection()
{
    // perform another check against the user data
    if( slotCheckFormData() )
        doDatabaseConnection();

}

void DatabaseConnectionDialog::setDatabaseName(const QString &dbName)
{
    editDatabaseName->setText( dbName );
}

void DatabaseConnectionDialog::setDatabasePortNumber(int &portNumber)
{
    spinBoxDatabasePort->setValue( portNumber );
}

void DatabaseConnectionDialog::setDatabaseHostName(const QString &hostname)
{
    editDatabaseHostName->setText( hostname );
}

void DatabaseConnectionDialog::setDatabaseUsername(const QString &username)
{
    editDatabaseUsername->setText( username );
}

void DatabaseConnectionDialog::setDatabaseDriverName(const QString &drvName)
{
    int index = comboDatabaseDriverName->findText( drvName );
    if( index >= 0 )
        comboDatabaseDriverName->setCurrentIndex( index );
}

void DatabaseConnectionDialog::setDatabasePassword(const QString &pwd)
{
    editDatabasePassword->setText( pwd );
}

void DatabaseConnectionDialog::run(bool autoConnect)
{
    bool statusOk = checkFormData();
    if( ! autoConnect || ! statusOk )
        exec();
    else
        doDatabaseConnection();
}
