#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/options.h>
#include <pybind11/stl.h>

#include <innermodel/innermodel.h>
#include <innermodel/innermodelnode.h>
#include <innermodel/innermodelviewer.h>

// class Foo {
//   public:
//   Foo(
//       const int poly,
//       const std::array<double, 3> & direction,
//       const double alpha = 0.0,
//       const double edge_size = 0.0
//       )
//   {}
// };

namespace py = pybind11;

PYBIND11_MODULE(inner_model, m) {
    py::class_<QString>(m, "QString")
        .def(py::init<>())
        .def(py::init<const char *>())
        .def("toStdString", &QString::toStdString)
        .def("__repr__", [](const QString &a) { return a.toStdString(); });
    
    py::class_<InnerModel, std::shared_ptr<InnerModel>>(m, "InnerModel")
        .def(py::init<>())
        .def(py::init<std::string>())
        .def(py::init<InnerModel *>())
        .def("update", &InnerModel::update)
        .def("cleanupTables", &InnerModel::cleanupTables)
        // cannot use these methods as binding of RTMat is not available
        .def("getTransformationMatrix", (void (InnerModel::*)(const QString &, const QString &)) 
                                                            &InnerModel::getTransformationMatrix)
        .def("getTransformationMatrixS", (void (InnerModel::*)(const std::string &, const std::string &)) 
                                                            &InnerModel::getTransformationMatrixS)
        .def("getParentIdentifier", &InnerModel::getParentIdentifier)
        .def("getParentIdentifierS", &InnerModel::getParentIdentifierS)
        .def("printModel", &InnerModel::print, py::arg("id")=QString(""))
        .def("updateTransformValues", (void (InnerModel::*)(QString, float, float, float, float, 
                float, float, QString)) &InnerModel::updateTransformValues, py::arg("transformId"), 
                py::arg("tx"), py::arg("ty"), py::arg("tz"), py::arg("rx"), py::arg("ry"), 
                py::arg("rz"), py::arg("parentId")=QString(""))
        .def("updateTransformValuesS", (void (InnerModel::*)(std::string, float, float, float, float, 
                float, float, std::string)) &InnerModel::updateTransformValuesS, py::arg("transformId"), 
                py::arg("tx"), py::arg("ty"), py::arg("tz"), py::arg("rx"), py::arg("ry"), 
                py::arg("rz"), py::arg("parentId")=std::string(""))
        .def("updateTranslationValues", &InnerModel::updateTranslationValues, py::arg("transformId"), 
                        py::arg("tx"), py::arg("ty"), py::arg("tz"), py::arg("parentId")=QString(""))
        .def("updateRotationValues", &InnerModel::updateRotationValues, py::arg("transformId"), 
                        py::arg("rx"), py::arg("ry"), py::arg("rz"), py::arg("parentId")=QString(""))
        .def("updateJointValue", &InnerModel::updateJointValue, py::arg("jointId"), 
                                                        py::arg("angle"), py::arg("force")=false)
        .def("updatePrismaticJointPosition", &InnerModel::updatePrismaticJointPosition)
        .def("updatePlaneValues", &InnerModel::updatePlaneValues)
        .def("removeNode", &InnerModel::removeNode)
        .def("__repr__", [](const InnerModel &a) { return "class InnerModel"; });

    py::class_<osg::Group, std::unique_ptr<osg::Group, py::nodelete>>(m, "osgGroup")
        .def(py::init<>())
        .def("__repr__", [](const osg::Group &a) { return "class osg::Group"; });

    // a virtual class, derived by all types of nodes
    py::class_<InnerModelNode>(m, "InnerModelNode")
        // .def(py::init<QString>())
        // .def(py::init<QString, InnerModelNode*>())
        .def("treePrint", &InnerModelNode::treePrint, py::arg("s"), py::arg("verbose")=false)
        .def("setParent", &InnerModelNode::setParent)
        .def("addChild", &InnerModelNode::addChild)
        .def("setFixed", &InnerModelNode::setFixed, py::arg("f")=true)
        .def("isFixed", &InnerModelNode::isFixed)
        .def("updateChildren", &InnerModelNode::updateChildren)
        .def("print", &InnerModelNode::print)
        .def("update", &InnerModelNode::update)
        // need to bind QTextStream before we use this method
        .def("save", &InnerModelNode::save)
        .def_readonly("id", &InnerModelNode::id)
        .def_readonly("level", &InnerModelNode::level)
        .def_readonly("fixed", &InnerModelNode::fixed)
        .def_readonly("innermodel", &InnerModelNode::innerModel)
        .def_readonly("parent", &InnerModelNode::parent)
        .def("__repr__", [](const InnerModelNode &a) { return "class InnerModelNode"; });

    py::class_<InnerModelTransform, InnerModelNode>(m, "InnerModelTransform")
        // .def(py::init<QString>())
        // .def(py::init<QString, InnerModelNode*>())
        .def(py::init<QString, QString, float, float, float, float, float, float, float>())
        .def(py::init<QString, QString, float, float, float, float, float, float, float, InnerModelNode *>())
        .def("print", &InnerModelTransform::print)
        // need to bind QTextStream first in order to use `save`
        .def("save", &InnerModelTransform::save)
        .def("update", (void (InnerModelTransform::*)()) &InnerModelTransform::update)
        .def("update", (void (InnerModelTransform::*)(float, float, float, float, float, float)) 
                                                                    &InnerModelTransform::update)
        .def_readwrite("mass", &InnerModelTransform::mass)
        .def_readwrite("engine", &InnerModelTransform::engine)
        .def_readwrite("backtX", &InnerModelTransform::backtX)
        .def_readwrite("backtY", &InnerModelTransform::backtY)
        .def_readwrite("backtZ", &InnerModelTransform::backtZ)
        .def_readwrite("backrX", &InnerModelTransform::backrX)
        .def_readwrite("backrY", &InnerModelTransform::backrY)
        .def_readwrite("backrZ", &InnerModelTransform::backrZ)
        .def("__repr__", [](const InnerModelTransform &a) { return "class InnerModelTransform"; });

    py::class_<InnerModelViewer>(m, "InnerModelViewer")
        .def(py::init<InnerModel *>())
        .def(py::init<InnerModel *, QString>())
        .def(py::init<InnerModel *, QString, osg::Group *>())
        .def(py::init<InnerModel *, QString, osg::Group *, bool>())
        // .def(py::init<InnerModel *, QString, osg::Group *, bool>(),
        //                                 py::arg("im"), py::arg("root")="root", 
        //                                 py::arg("parent")=NULL, py::arg("ignoreCameras")ss=false)
        .def(py::init<const std::shared_ptr<InnerModel>&>())
        .def(py::init<const std::shared_ptr<InnerModel>&, QString>())
        .def(py::init<const std::shared_ptr<InnerModel>&, QString, osg::Group *>())
        .def(py::init<const std::shared_ptr<InnerModel>&, QString, osg::Group *, bool>())
        // .def(py::init<const std::shared_ptr<InnerModel>&, QString, osg::Group *, bool>(),
        //                                 py::arg("im"), py::arg("root")="root", 
        //                                 py::arg("parent")=NULL, py::arg("ignoreCameras")=false)
        .def("update", &InnerModelViewer::update)
        .def("recursiveConstructor", (void (InnerModelViewer::*)(InnerModelNode*)) 
                                                            &InnerModelViewer::recursiveConstructor)
        .def("recursiveRemove", &InnerModelViewer::recursiveRemove)
        .def("setMainCamera", &InnerModelViewer::setMainCamera)
        .def("__repr__", [](const InnerModelViewer &a) { return "class InnerModelViewer"; });

    // py::class_<Foo> c(m, "Foo");

    // c.def(py::init<const int, const std::array<double, 3>&>());
    // c.def(py::init<const int, const std::array<double, 3>&, const double>());
    // c.def(py::init<const int, const std::array<double, 3>&, const double, const double>());

    // c.def(py::init<const int, const std::array<double, 3>&, const double, const double>(), 
    //      py::arg("poly"), py::arg("direction"), py::arg("alpha")=0.0, py::arg("edge_size")=0.0);
}
