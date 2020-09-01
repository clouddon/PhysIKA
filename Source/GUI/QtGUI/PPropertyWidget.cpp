#include "PPropertyWidget.h"
#include "Framework/Module.h"
#include "Framework/Node.h"
#include "Framework/Framework/SceneGraph.h"

#include "PVTKOpenGLWidget.h"
#include "PCustomWidgets.h"

#include "vtkRenderer.h"
#include <vtkRenderWindow.h>

#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

namespace PhysIKA
{
	QBoolFieldWidget::QBoolFieldWidget(Field* field)
		: QGroupBox()
	{
		m_field = field;
		VarField<bool>* f = TypeInfo::CastPointerDown<VarField<bool>>(m_field);
		if (f == nullptr)
		{
			return;
		}

		this->setStyleSheet("border:none");
		QGridLayout* layout = new QGridLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		this->setLayout(layout);

		QLabel* name = new QLabel();
		name->setFixedSize(160, 18);
		name->setText(QString::fromStdString(field->getObjectName()));
		QCheckBox* checkbox = new QCheckBox();
		checkbox->setFixedSize(40, 18);
		layout->addWidget(name, 0, 0);
		layout->addWidget(checkbox, 0, 1, Qt::AlignRight);

		connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(changeValue(int)));

		checkbox->setChecked(f->getValue());
	}


	void QBoolFieldWidget::changeValue(int status)
	{
		VarField<bool>* f = TypeInfo::CastPointerDown<VarField<bool>>(m_field);
		if (f == nullptr)
		{
			return;
		}

		if (status == Qt::Checked)
		{
			f->setValue(true);
		}
		else if (status == Qt::PartiallyChecked)
		{
			//m_pLabel->setText("PartiallyChecked");
		}
		else
		{
			f->setValue(false);
			//m_pLabel->setText("Unchecked");
		}

		emit fieldChanged();
	}

	QIntegerFieldWidget::QIntegerFieldWidget(Field* field)
		: QGroupBox()
	{
		m_field = field;
		VarField<int>* f = TypeInfo::CastPointerDown<VarField<int>>(m_field);
		if (f == nullptr)
		{
			return;
		}

		this->setStyleSheet("border:none");
		QGridLayout* layout = new QGridLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		this->setLayout(layout);

		QLabel* name = new QLabel();
		name->setFixedSize(100, 18);
		name->setText(QString::fromStdString(field->getObjectName()));

		QSpinBox* spinner = new QSpinBox;
		spinner->setFixedSize(120, 18);
		spinner->setValue(f->getValue());

		layout->addWidget(name, 0, 0);
		layout->addWidget(spinner, 0, 1, Qt::AlignRight);

		this->connect(spinner, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
	}

	void QIntegerFieldWidget::changeValue(int value)
	{
		emit fieldChanged();
	}


	QRealFieldWidget::QRealFieldWidget(Field* field)
		: QGroupBox()
	{
		m_field = field;
		
		this->setStyleSheet("border:none");
		QGridLayout* layout = new QGridLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		this->setLayout(layout);

		QLabel* name = new QLabel();
		name->setFixedSize(80, 18);
		name->setText(QString::fromStdString(field->getObjectName()));

		QDoubleSlider* slider = new QDoubleSlider;
		slider->setFixedSize(80,18);
		slider->setRange(0.0, 1.0);

		QDoubleSpinner* spinner = new QDoubleSpinner;
		spinner->setFixedSize(60, 18);
		spinner->setRange(0.0, 1.0);

		layout->addWidget(name, 0, 0);
		layout->addWidget(slider, 0, 1);
		layout->addWidget(spinner, 0, 2, Qt::AlignRight);

		QObject::connect(slider, SIGNAL(valueChanged(double)), spinner, SLOT(setValue(double)));
		QObject::connect(spinner, SIGNAL(valueChanged(double)), slider, SLOT(setValue(double)));
		QObject::connect(spinner, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));

		std::string template_name = field->getTemplateName();
		if (template_name == std::string(typeid(float).name()))
		{
			VarField<float>* f = TypeInfo::CastPointerDown<VarField<float>>(m_field);
			slider->setValue((double)f->getValue());
		}
		else if(template_name == std::string(typeid(double).name()))
		{
			VarField<double>* f = TypeInfo::CastPointerDown<VarField<double>>(m_field);
			slider->setValue(f->getValue());
		}
	}

	void QRealFieldWidget::changeValue(double value)
	{
		std::string template_name = m_field->getTemplateName();

		if (template_name == std::string(typeid(float).name()))
		{
			VarField<float>* f = TypeInfo::CastPointerDown<VarField<float>>(m_field);
			f->setValue((float)value);
		}
		else if (template_name == std::string(typeid(double).name()))
		{
			VarField<double>* f = TypeInfo::CastPointerDown<VarField<double>>(m_field);
			f->setValue(value);
		}

		emit fieldChanged();
	}


	QVector3FieldWidget::QVector3FieldWidget(Field* field)
		: QGroupBox()
	{
		m_field = field;

		this->setStyleSheet("border:none");
		QGridLayout* layout = new QGridLayout;
		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		this->setLayout(layout);

		QLabel* name = new QLabel();
		name->setFixedSize(80, 18);
		name->setText(QString::fromStdString(field->getObjectName()));

		spinner1 = new QDoubleSpinner;
		spinner1->setFixedSize(60, 18);
		spinner1->setRange(0.0, 1.0);

		spinner2 = new QDoubleSpinner;
		spinner2->setFixedSize(60, 18);
		spinner2->setRange(0.0, 1.0);

		spinner3 = new QDoubleSpinner;
		spinner3->setFixedSize(60, 18);
		spinner3->setRange(0.0, 1.0);

		layout->addWidget(name, 0, 0);
		layout->addWidget(spinner1, 0, 1);
		layout->addWidget(spinner2, 0, 2);
		layout->addWidget(spinner3, 0, 3);

		QObject::connect(spinner1, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
		QObject::connect(spinner2, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
		QObject::connect(spinner3, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));

		//std::string template_name = field->getTemplateName();
		//if (template_name == std::string(typeid(float).name()))
		//{
		//	VarField<float>* f = TypeInfo::CastPointerDown<VarField<float>>(m_field);
		//	slider->setValue((double)f->getValue());
		//}
		//else if (template_name == std::string(typeid(double).name()))
		//{
		//	VarField<double>* f = TypeInfo::CastPointerDown<VarField<double>>(m_field);
		//	slider->setValue(f->getValue());
		//}
	}


	void QVector3FieldWidget::changeValue(double value)
	{
		double v1 = spinner1->value();
		double v2 = spinner2->value();
		double v3 = spinner3->value();

		std::string template_name = m_field->getTemplateName();

		if (template_name == std::string(typeid(Vector3f).name()))
		{
			VarField<Vector3f>* f = TypeInfo::CastPointerDown<VarField<Vector3f>>(m_field);
			f->setValue(Vector3f((double)v1, (double)v2, (double)v3));
		}
		else if (template_name == std::string(typeid(Vector3d).name()))
		{
			VarField<Vector3d>* f = TypeInfo::CastPointerDown<VarField<Vector3d>>(m_field);
			f->setValue(Vector3d(v1, v2, v3));
		}

		emit fieldChanged();
	}

	//QWidget-->QVBoxLayout-->QScrollArea-->QWidget-->QGridLayout
	PPropertyWidget::PPropertyWidget(QWidget *parent)
		: QWidget(parent)
		, m_main_layout()
	{
		m_main_layout = new QVBoxLayout;
		m_scroll_area = new QScrollArea;

		m_main_layout->setContentsMargins(0, 0, 0, 0);
		m_main_layout->setSpacing(0);
		m_main_layout->addWidget(m_scroll_area);

		m_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		m_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		m_scroll_area->setWidgetResizable(true);

		m_scroll_layout = new QGridLayout;
		m_scroll_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

		QWidget * m_scroll_widget = new QWidget;
		m_scroll_widget->setLayout(m_scroll_layout);
		
		m_scroll_area->setWidget(m_scroll_widget);


		setMinimumWidth(250);
		setLayout(m_main_layout);
	}

	PPropertyWidget::~PPropertyWidget()
	{
		m_widgets.clear();
	}

	QSize PPropertyWidget::sizeHint() const
	{
		return QSize(20, 20);
	}

	QWidget* PPropertyWidget::addWidget(QWidget* widget)
	{
		m_scroll_layout->addWidget(widget);
		m_widgets.push_back(widget);

		return widget;
	}

	void PPropertyWidget::removeAllWidgets()
	{
		//TODO: check whether m_widgets[i] should be explicitly deleted
		for (int i = 0; i < m_widgets.size(); i++)
		{
			m_scroll_layout->removeWidget(m_widgets[i]);
			delete m_widgets[i];
		}
		m_widgets.clear();
	}

	void PPropertyWidget::showProperty(Module* module)
	{
//		clear();

		updateContext(module);
	}

	void PPropertyWidget::showProperty(Node* node)
	{
//		clear();

		updateContext(node);
	}

	void PPropertyWidget::updateDisplay()
	{
//		PVTKOpenGLWidget::getCurrentRenderer()->GetActors()->RemoveAllItems();
		SceneGraph::getInstance().draw();
		PVTKOpenGLWidget::getCurrentRenderer()->GetRenderWindow()->Render();
	}

	void PPropertyWidget::updateContext(Base* base)
	{
		if (base == nullptr)
		{
			return;
		}

		this->removeAllWidgets();

		std::vector<Field*>& fields = base->getAllFields();

		for each (Field* var in fields)
		{
			if (var != nullptr)
			{
				if (var->getClassName() == std::string("Variable"))
				{
					addScalarFieldWidget(var);
				}
				else if (var->getClassName() == std::string("ArrayBuffer"))
				{
				}
				//addItem(new QListWidgetItem(var->getObjectName().c_str(), this));
			}
		}
	}

	void PPropertyWidget::addScalarFieldWidget(Field* field)
	{
		std::string template_name = field->getTemplateName();
		if (template_name == std::string(typeid(bool).name()))
		{
			auto fw = new QBoolFieldWidget(field);
			this->connect(fw, SIGNAL(fieldChanged()), this, SLOT(updateDisplay()));

			this->addWidget(fw);
		}
		else if (template_name == std::string(typeid(int).name()))
		{
			auto fw = new QIntegerFieldWidget(field);
			this->connect(fw, SIGNAL(fieldChanged()), this, SLOT(updateDisplay()));

			this->addWidget(fw);
//			this->addWidget(new QIntegerFieldWidget(new VarField<int>()));
		}
		else if (template_name == std::string(typeid(float).name()))
		{
			this->addWidget(new QRealFieldWidget(field));
		}
		else if (template_name == std::string(typeid(Vector3f).name()))
		{
			this->addWidget(new QVector3FieldWidget(field));
		}
	}

	void PPropertyWidget::addArrayFieldWidget(Field* field)
	{

	}

}
