#ifndef CONVERTLIST_H
#define CONVERTLIST_H

#include <QWidget>
#include <QVector>
#include <QSharedPointer>
#include "converter/conversionparameters.h"

class MediaConverter;
class MediaProbe;
class QTreeWidget;
class QTreeWidgetItem;

class ConvertList : public QWidget
{
    Q_OBJECT
public:

    struct Task
    {
        enum TaskStatus { QUEUED, RUNNING, FINISHED, FAILED };
        int id;
        TaskStatus status;
        ConversionParameters param;
        QTreeWidgetItem *listitem;
    };

    explicit ConvertList(QWidget *parent = 0);
    ~ConvertList();

    /*! Append a task to the list
     * @param param the conversion parameter including the source and destination filename.
     * @return If the function succeeds, it returns true.
     *  Otherwise, it returns false.
     */
    bool addTask(const ConversionParameters &param);

    /*! Append a list of tasks to the list
     * @param paramList the list of conversion parameters
     * @return the number of successfully added tasks.
     */
    int addTasks(const QList<ConversionParameters>& paramList);

    void removeTask(int index);

    bool isBusy() const;
    bool isEmpty() const;
    int count() const;
    int selectedCount() const;

    /*! Returns the pointer to the conversion parameter of the currently selected item.
     *  @return If the function fails, it returns NULL.
     */
    const ConversionParameters* getCurrentIndexParameter() const;

signals:
    void start_conversion(int index, ConversionParameters param);
    void task_finished(int);
    void all_tasks_finished();

public slots:

    /*! Start the conversion progress.
     *  If another task is being processed, the function does nothing.
     */
    void start();

    /*! Stop the conversion progress
     */
    void stop();

    /*! Remove all selected tasks but quietly ignore tasks in progress.
     */
    void removeSelectedItems();

    /*! Remove all tasks marked as completed.
     */
    void removeCompletedItems();

    /*! Popup edit-parameter dialog.
     *  The parameter of the first selected task will be used as the default
     *  configuration. If the user presses OK in the dialog, all selected
     *  tasks will be set to the same parameter.
     */
    void editSelectedParameters();

    /*! Mark selected items as queued so that they will be converted again.
     *  If the converter is idle, start converting those items.
     *  Otherwise, the items are simply marked as queued.
     */
    void retrySelectedItems();

    void retryAll();

    /*! Remove all tasks but quietly ignore tasks that are in progress.
     */
    void clear();

private slots:
    void task_finished_slot(int);
    void progress_refreshed(int);

protected:
    bool list_keyPressEvent(QKeyEvent *event);
    void list_dragEnterEvent(QDragEnterEvent *event);
    void list_dragMoveEvent(QDragMoveEvent *event);
    void list_dragLeaveEvent(QDragLeaveEvent *event);
    void list_dropEvent(QDropEvent *event);

private:
    Q_DISABLE_COPY(ConvertList)

    class ListEventFilter;
    friend class ListEventFilter;
    typedef QSharedPointer<Task> TaskPtr;

    QTreeWidget *m_list;
    ListEventFilter *m_listEventFilter;
    QVector<TaskPtr> m_tasks;
    int prev_index;
    void init_treewidget(QTreeWidget*);
    MediaConverter *m_converter;
    MediaProbe *m_probe;
    Task *m_current_task;
    bool is_busy;
    unsigned int m_progress_column_index;
    void reset_item(int index);
    void remove_items(const QList<QTreeWidgetItem*>& itemList);
};

#endif // CONVERTLIST_H
