import innermodel_pybind11 as im
import unittest

class TestStringMethods(unittest.TestCase):

    # something along the lines of:
    # assertEqual(), assertTrue(), assertFalse()

    def test_qstring(self):
        qstr1 = im.QString()
        qstr2 = im.QString('example')

        self.assertTrue(qstr1 is not None)
        self.assertTrue(qstr1 is not None)
        self.assertTrue(qstr2.toStdString() == 'example')
        self.assertTrue(type(qstr1) is im.inner_model.QString)
        self.assertTrue(type(qstr2) is im.inner_model.QString)
        self.assertTrue(type(qstr2.toStdString()) is str)

    def test_innermodel(self):
        im1 = im.InnerModel()
        im2 = im.InnerModel('/home/robocomp/robocomp/files/innermodel/simplesimpleworld.xml')
        im3 = im.InnerModel(im1)
        im4 = im.InnerModel(im2)

        # test whether all the objects are not None
        self.assertTrue(im1 is not None)
        self.assertTrue(im2 is not None)
        self.assertTrue(im3 is not None)
        self.assertTrue(im4 is not None)

        # to show that im4 has truly copied im2
        self.assertTrue(im2.getParentIdentifierS("caja1") == im4.getParentIdentifierS("caja1"))
        self.assertTrue(im2.getParentIdentifierS("floor") == im4.getParentIdentifierS("floor"))

        # to show that im2 has properly read the xml file
        self.assertTrue(im2.getParentIdentifierS("caja1") == "world")

        # test that getParentIdentifier has two variants (QString and str)
        self.assertTrue(type(im2.getParentIdentifierS("caja1")) is str)
        self.assertTrue(type(im2.getParentIdentifier(im.QString("caja1"))) is im.inner_model.QString)

        # test for removeNode and parentIdentifier
        im2.removeNode(im.QString("caja1"))
        self.assertTrue(im2.getParentIdentifierS("caja1") == "")

    def test_innermodelnode(self):
        # raise a typeerror when trying to instantiate because it doesn't have constructor
        with self.assertRaises(TypeError):
            im.InnerModelNode()

    def test_innermodeltransform(self):
        # raise error when it doesn't follow the constructor syntax
        with self.assertRaises(TypeError):
            im.InnerModelTransform()
        self.assertTrue(im.InnerModelTransform(im.QString(""), im.QString(""), 0, 0, 0, 0, 0, 0, 0) is not None)

    def test_osggroup(self):
        # test that the object is not None
        osg = im.osgGroup()
        self.assertTrue(osg is not None)

    def test_innermodelviewer(self):
        im_ = im.InnerModel()
        imv1 = im.InnerModelViewer(im_)
        imv2 = im.InnerModelViewer(im_, im.QString("root"))
        imv3 = im.InnerModelViewer(im_, im.QString("root"), im.osgGroup())
        imv4 = im.InnerModelViewer(im_, im.QString("root"), im.osgGroup(), False)

        with self.assertRaises(TypeError):
            imv0 = im.InnerModelViewer()

        self.assertTrue(imv1 is not None)
        self.assertTrue(imv2 is not None)
        self.assertTrue(imv3 is not None)
        self.assertTrue(imv4 is not None)

if __name__ == '__main__':
    unittest.main()
