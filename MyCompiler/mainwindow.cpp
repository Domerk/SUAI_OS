#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MyLexer = new lexer();
    MyParser = new parser();
    LexTableView = new QLabel();
    TreeView = new QLabel();

    QFont font = LexTableView->font();
    font.setPixelSize(12);
    LexTableView->setFont(font);
    LexTableView->setWindowTitle(tr("Таблица лексем"));
    LexTableView->setAlignment(Qt::AlignCenter);

    TreeView->setFont(font);
    TreeView->setWindowTitle(tr("Дерево вывода"));
    TreeView->setAlignment(Qt::AlignCenter);

    messbox = new QMessageBox();
    messbox->setFont(font);

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);

    connect(this, SIGNAL(toLexer(QString)), MyLexer, SLOT(textSlot(QString)));
    connect (MyLexer, SIGNAL(tableSignal(QString)), this, SLOT(resultsOfLexer(QString)));
    connect (MyLexer, SIGNAL(failSignal(QString)), this, SLOT(lexicalError(QString)));
    connect (MyLexer, SIGNAL(lexTableSignal(QVector<Lexeme>)), MyParser, SLOT(lexTableSlot(QVector<Lexeme>)));
    connect (this, SIGNAL(startParser()), MyParser, SLOT(startParser()));
    connect (MyParser, SIGNAL(parsError(QString)), this, SLOT(parsError(QString)));
    connect (MyParser, SIGNAL(parsResult(QString)), this, SLOT(parsResult(QString)));

}

MainWindow::~MainWindow()
{
    delete MyLexer;
    delete MyParser;
    delete LexTableView;
    delete messbox;
    delete TreeView;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->textEdit->toPlainText();
    emit toLexer(text);

    if (!LexTableView->isHidden())
        LexTableView->hide();
}

void MainWindow::on_pushButton_2_clicked()
{
    LexTableView->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    emit startParser();
}

void MainWindow::resultsOfLexer(QString forTable)
{
    LexTableView->setText(forTable);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}

void MainWindow::lexicalError(QString txt)
{
    messbox->setText(tr("В ходе выполнения лексического анализа были обнаружены ошибки!"));
    messbox->setWindowTitle(tr("Сообщение об ошибке"));
    messbox->show();

    ui->textEdit->setHtml(txt); // заменяем исходный текст его копией с выделенными ошибками
    ui->pushButton_3->setEnabled(false);
}

void MainWindow::parsError(QString errorTxt)
{
    errorTxt.prepend(tr("В ходе выполнения синтаксического анализа были обнаружены ошибки.<br>"));
    messbox->setText(errorTxt);
    messbox->setWindowTitle(tr("Сообщение об ошибке"));
    messbox->show();
}

void MainWindow::parsResult(QString tree)
{
    TreeView->setText(tree);
    ui->pushButton_4->setEnabled(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    TreeView->show();
}
