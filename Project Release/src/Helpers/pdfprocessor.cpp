#include "pdfprocessor.h"
#include "debug.h"

#ifdef USINGPOPPLER

/**
 * Method: PDFProcessor()
 *
 * Description:
 *      basic constructor for the class
 *
 * Attributes:
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
PDFProcessor::PDFProcessor() {
    current_page = 0;
    document = 0;
    window = 0;
    zoom_percent = 1;
}

/**
 * Method: PDFProcessor(MainWindow * _window)
 *
 * Description:
 *      Constructor that sets the window member of the PDFProcessor
 *
 * Attributes:
 *      MainWindow * _window - pointer to the main window (application container)
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
PDFProcessor::PDFProcessor(MainWindow * _window) {
    current_page = 0;
    document = 0;
    window = _window;
    zoom_percent = 1;
}

/**
 * Method: PDFProcessor(QString file, MainWindow * _window)
 *
 * Description:
 *      Constructor that sets the pdf file and the window
 *
 * Attributes:
 *      QString file - path and filename of the pdf to open
 *      MainWindow * _window - pointer to the main window (application container)
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
PDFProcessor::PDFProcessor(QString file, MainWindow * _window) {
    document = Poppler::Document::load(file);
    current_page = 0;
    window = _window;
    file_name = file;
    zoom_percent = 1;
}

/**
 * Method: LoadDocument(QString file)
 *
 * Description:
 *      loads a pdf document from the file
 *
 * Attributes:
 *      QString file - path and filename of the pdf to open
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
bool PDFProcessor::LoadDocument(QString file) {
    bool success = true;
    if (document != 0) {
        success = CloseDocument();
    }
    if (success) {
        document = Poppler::Document::load(file);

        document->setRenderHint(Poppler::Document::TextAntialiasing);

        file_name = file;
        success = false;
        if (document != 0) {
            success = true;
        }
        return success;
    }
    return false;
}

/**
 * Method: CloseDocument()
 *
 * Description:
 *      Closes the pdf document
 *
 * Attributes:
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
bool PDFProcessor::CloseDocument() {
    delete document;
    document = 0;
    if (document == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * Method: SaveDocument(QString fileName)
 *
 * Description:
 *      Saves the document at the fileName
 *
 * Attributes:
 *      QString fileName - path and filename of the pdf to save
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
bool PDFProcessor::SaveDocument(QString fileName) {
    Poppler::PDFConverter *pdfConv = document->pdfConverter();
    pdfConv->setOutputFileName(fileName);
    pdfConv->setPDFOptions(pdfConv->pdfOptions()|Poppler::PDFConverter::WithChanges);
    bool success = pdfConv->convert();
    delete pdfConv;

    return success;
}

/**
 * Method: SaveDocument()
 *
 * Description:
 *      Saves the document if the file_name member is set
 *
 * Attributes:
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
bool PDFProcessor::SaveDocument() {
    Poppler::PDFConverter *pdfConv = document->pdfConverter();
    pdfConv->setOutputFileName("file_name.pdf");
    pdfConv->setPDFOptions(pdfConv->pdfOptions()|Poppler::PDFConverter::WithChanges);
    bool success = pdfConv->convert();
    delete pdfConv;

    return success;
}

/**
 * Method: getAnnotations()
 *
 * Description:
 *      returns the annotations/comments attached to the pdf document
 *
 * Attributes:
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QList<Poppler::Annotation * > PDFProcessor::getAnnotations() {
    Poppler::Page * page = document->page(current_page);
    QList<Poppler::Annotation * > annotations = page->annotations();
    return annotations;
}

/**
 * Method: CreateAnnotation(QString text, double x, double y, double width, double height)
 *
 * Description:
 *      creates and returns a new annotation/comment
 *
 * Attributes:
 *      QString text - The text for the comment
 *      double x - The x position for the comment on the document (as a percent of the document's width)
 *      double y - The y position for the comment on the document (as a percent of the document's height)
 *      double width - The width for the comment box (as a percent of the document's width)
 *      double height - The height for the comment box (as a percent of the document's height)
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
Poppler::Annotation * PDFProcessor::CreateAnnotation(QString text, double x, double y, double width, double height) {
    Poppler::TextAnnotation * annotation = new Poppler::TextAnnotation(Poppler::TextAnnotation::InPlace);
    annotation->setContents(text);
    annotation->setBoundary(QRectF(x, y, width, height));
    return annotation;
}

/**
 * Method: AddAnnotation(Poppler::Annotation * _annotation)
 *
 * Description:
 *      adds an annotation/comment to the current pdf document
 *
 * Attributes:
 *      Poppler::Annotation * _annotation - The pointer to a new annotation object
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
void PDFProcessor::AddAnnotation(Poppler::Annotation * _annotation) {
    Poppler::Page * page = document->page(current_page);
    Poppler::Annotation::Style style;
    style.setColor(QColor(255, 255, 0));
    _annotation->setStyle(style);
    page->addAnnotation(_annotation);
    delete page;
}

/**
 * Method: RemoveAnnotation(Poppler::Annotation * _annotation)
 *
 * Description:
 *      removes an annotation/comment from the pdf document
 *
 * Attributes:
 *      Poppler::Annotation * _annotation - The pointer to an annotation that is attached to the document
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
void PDFProcessor::RemoveAnnotation(Poppler::Annotation * _annotation) {
    Poppler::Page * page = document->page(current_page);
    page->removeAnnotation(_annotation);
    delete page;
}

/**
 * Method: ModifyAnnotation(Poppler::Annotation * _annotation, QString text, double x, double y, double width, double height)
 *
 * Description:
 *      modifies an annotation attached to the pdf document
 *
 * Attributes:
 *      Poppler::Annotation * _annotation - The pointer to the annotation to modify.
 *      QString text - The text for the comment
 *      double x - The x position for the comment on the document (as a percent of the document's width)
 *      double y - The y position for the comment on the document (as a percent of the document's height)
 *      double width - The width for the comment box (as a percent of the document's width)
 *      double height - The height for the comment box (as a percent of the document's height)
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
void PDFProcessor::ModifyAnnotation(Poppler::Annotation * _annotation, QString text, double x, double y, double width, double height) {
    _annotation->setContents(text);
    _annotation->setBoundary(QRectF(x, y, width, height));
}

/**
 * Method: getPDFImage()
 *
 * Description:
 *      returns a displayable image of the PDF document
 *
 * Attributes:
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
QGraphicsScene * PDFProcessor::getPDFImage() {
    if (document != 0 && window != 0) {
        Poppler::Page* pdfPage = document->page(current_page);
        QImage image = pdfPage->renderToImage(window->physicalDpiX() * zoom_percent, window->physicalDpiY() * zoom_percent);
        QPixmap map = QPixmap::fromImage(image);
        QGraphicsScene * scene = new QGraphicsScene;
        scene->addPixmap(map);
        delete pdfPage;
        return scene;
    }
    return 0;
}

/**
 * Method: setZoomPercent(double _percent)
 *
 * Description:
 *      zooms in and out on the document image
 *
 * Attributes:
 *      double _percent - the percent to zoom the document image
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
void PDFProcessor::setZoomPercent(double _percent) {
    zoom_percent = _percent;
}

/**
 * Method: ChangePage(int pageNumber)
 *
 * Description:
 *      changes the page of the pdf file
 *
 * Attributes:
 *      int pageNumber - the pagenumber for the document to change to
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
void PDFProcessor::ChangePage(int pageNumber) {
    if (pageNumber >= 0) {
        current_page = pageNumber;
    }
}

/**
 * Method: getCurrentPageNumber()
 *
 * Description:
 *      returns the current pdf's page
 *
 * Attributes:
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
int PDFProcessor::getCurrentPageNumber() {
    return current_page;
}

/**
 * Method: setWindow(MainWindow *_window)
 *
 * Description:
 *      sets the window member of the PDFProcessor
 *
 * Attributes:
 *      MainWindow * _window - pointer to the main window (application container)
 *
 * Author: Joshua Campbell
 *
 * Version: 1
 */
void PDFProcessor::setWindow(MainWindow *_window) {
    window = _window;
}
#endif
