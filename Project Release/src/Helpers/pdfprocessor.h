#ifndef PDFPROCESSOR
#define PDFPROCESSOR

#define USINGPOPPLER

#include <poppler-qt5.h>

#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QList>
#include <QMap>
#include <QListIterator>

#include "mainwindow.h"
#ifdef USINGPOPPLER

/**
 * ClassName: PDFProcessor
 *
 * Description: Handles interactions between the application and the PDF document
 *
 * Methods:
 *      PDFProcessor()
 *           basic constructor for the class
 *
 *      PDFProcessor(MainWindow * _window)
 *           constructor that sets the window member of the PDFProcessor
 *
 *      PDFProcessor(QString file, MainWindow * _window)
 *           constructor that sets the pdf file and the window
 *
 *      bool LoadDocument(QString file)
 *           loads a pdf document from the file
 *
 *      bool CloseDocument()
 *           closes the pdf document
 *
 *      bool SaveDocument(QString fileName)
 *           Saves the document at the fileName
 *
 *      bool SaveDocument()
 *           Saves the document if the file_name member is set
 *
 *      void ChangePage(int pageNumber)
 *           changes the page of the pdf file
 *
 *      int getCurrentPageNumber()
 *           returns the current pdf's page
 *
 *      QGraphicsScene * getPDFImage()
 *           returns a displayable image of the PDF document
 *
 *      void setZoomPercent(double _percent)
 *           zooms in and out on the document image
 *
 *      QList<Poppler::Annotation * > getAnnotations()
 *           returns the annotations/comments attached to the pdf document
 *
 *      Poppler::Annotation * CreateAnnotation(QString text, double x, double y, double width, double height)
 *           creates and returns a new annotation/comment
 *
 *      void AddAnnotation(Poppler::Annotation * _annotation)
 *           adds an annotation/comment to the current pdf document
 *
 *      void RemoveAnnotation(Poppler::Annotation * _annotation)
 *           removes an annotation/comment from the pdf document
 *
 *      void ModifyAnnotation(Poppler::Annotation * _annotation, QString text, double x, double y, double width, double height)
 *           modifies an annotation attached to the pdf document
 *
 *      void setWindow(MainWindow * _window)
 *           sets the window member of the PDFProcessor
 *
 * Members:
 *      Poppler::Document * document - the PDF document
 *      double zoom_percent - the zoom percent for the pdf image
 *      int current_page - the current page of the pdf document
 *      QString file_name - the filename and path for the pdf document
 *      MainWindow * window - the main window pointer for getting the page width and height
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
class PDFProcessor {
public:
    PDFProcessor();
    PDFProcessor(MainWindow * _window);
    PDFProcessor(QString file, MainWindow * _window);

    //File Operations
    bool LoadDocument(QString file);
    bool CloseDocument();
    bool SaveDocument(QString fileName);
    bool SaveDocument();

    //Pagination
    void ChangePage(int pageNumber);
    int getCurrentPageNumber();

    //Rendering
    QGraphicsScene * getPDFImage();
    void setZoomPercent(double _percent);

    //Annotating
    QList<Poppler::Annotation * > getAnnotations();
    Poppler::Annotation * CreateAnnotation(QString text, double x, double y, double width, double height);
    void AddAnnotation(Poppler::Annotation * _annotation);
    void RemoveAnnotation(Poppler::Annotation * _annotation);
    void ModifyAnnotation(Poppler::Annotation * _annotation, QString text, double x, double y, double width, double height);

    //Setters
    void setWindow(MainWindow * _window);

    //Members
    Poppler::Document * document;
    double zoom_percent;
    int current_page;
    QString file_name;
    MainWindow * window;
};
#endif
#endif // PDFPROCESSOR

