//--------------------------------------------------------------------------------------
//
//  File:       ServiceViewerApp.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the service viewer application class.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms,
//              with or without modification, are permitted provided that the following
//              conditions are met:
//                * Redistributions of source code must retain the above copyright
//                  notice, this list of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright
//                  notice, this list of conditions and the following disclaimer in the
//                  documentation and/or other materials provided with the
//                  distribution.
//                * Neither the name of the copyright holders nor the names of its
//                  contributors may be used to endorse or promote products derived
//                  from this software without specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created:    2014-05-08
//
//--------------------------------------------------------------------------------------

#include "ServiceViewerApp.h"
#include "ServiceEntity.h"

#include "ofAppRunner.h"
#include "ofBitmapFont.h"
#include "ofGraphics.h"
#include "ofMesh.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

// Note that openFrameworks defines a macro called 'check' :( which messes up other header files.
#undef check
#include "M+MUtilities.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wc++11-extensions"
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wshadow"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wweak-vtables"
#endif // defined(__APPLE__)
#include <yarp/os/Network.h>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

//#define TEST_GRAPHICS_ /* */

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the service viewer application class. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ServiceViewerApp::ServiceViewerApp(void) :
            inherited(), _altActive(false), _commandActive(false), _controlActive(false), _networkAvailable(false),
            _registryAvailable(false), _shiftActive(false)
{
} // ServiceViewerApp::ServiceViewerApp

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void ServiceViewerApp::dragEvent(ofDragInfo dragInfo)
{
    inherited::dragEvent(dragInfo);
} // ServiceViewerApp::dragEvent

void ServiceViewerApp::draw(void)
{
#if defined(TEST_GRAPHICS_)
    ofBackground(ofColor::white);
#else // ! defined(TEST_GRAPHICS_)
    ofBackgroundGradient(ofColor::white, ofColor::gray);
#endif // ! defined(TEST_GRAPHICS_)
    if (_networkAvailable)
    {
        for (EntityList::iterator it(_entities.begin()); _entities.end() != it; ++it)
        {
            ServiceEntity * anEntity = *it;
            
            if (anEntity && (! anEntity->isSelected()))
            {
                anEntity->draw();
            }
        }
        for (EntityList::iterator it(_entities.begin()); _entities.end() != it; ++it)
        {
            ServiceEntity * anEntity = *it;
            
            if (anEntity && anEntity->isSelected())
            {
                anEntity->draw();
            }
        }
    }
    else
    {
        string      title = "The YARP network is not running";
        ofRectangle bbox = ofBitmapStringGetBoundingBox(title, 0, 0);
        ofMesh &    mesh = ofBitmapStringGetMesh(title, (ofGetWidth() - bbox.width) / 2,
                                                 (ofGetHeight() - bbox.height) / 2);
        
        ofEnableAlphaBlending();
        ofSetColor(ofColor::black);
		ofBitmapStringGetTextureRef().bind();
        mesh.draw();
		ofBitmapStringGetTextureRef().unbind();
        ofDisableAlphaBlending();
    }
} // ServiceViewerApp::draw

void ServiceViewerApp::exit(void)
{
    inherited::exit();
} // ServiceViewerApp::exit

PortEntry * ServiceViewerApp::findPort(string name)
{
    PortEntry *       result;
    PortMap::iterator match(_ports.find(name));
    
    if (_ports.end() == match)
    {
        result = NULL;
    }
    else
    {
        result = match->second;
    }
    return result;
} // ServiceViewerApp::findPort

void ServiceViewerApp::forgetPort(PortEntry * aPort)
{
    if (aPort)
    {
        PortMap::iterator match(_ports.find(aPort->getName()));
        
        if (_ports.end() != match)
        {
            _ports.erase(match);
        }
    }
} // ServiceViewerApp::forgetPort

void ServiceViewerApp::gatherEntities(void)
{
#if defined(TEST_GRAPHICS_)
    ServiceEntity * anEntity = new ServiceEntity(this);
    
    anEntity->setup("blort");
    anEntity->addPort("/blort-out", PortEntry::kPortDirectionOutput);
    anEntity->addPort("/blort-in", PortEntry::kPortDirectionInput);
    anEntity->addPort("/blort-out2", PortEntry::kPortDirectionOutput);
    anEntity->setConnectionColor(ofColor::purple);
    
    _entities.push_back(anEntity);
    ofRectangle prevShape = anEntity->getShape();
    
    anEntity = new ServiceEntity(this);
    anEntity->setup("blorg");
    anEntity->addPort("/blorg", PortEntry::kPortDirectionInput);
    _entities.push_back(anEntity);
    anEntity = new ServiceEntity(this);
    anEntity->setup("blirg");
    anEntity->addPort("/blirg", PortEntry::kPortDirectionOutput);
    anEntity->setConnectionWidth(3);
    _entities.push_back(anEntity);
    PortEntry * blortOut = findPort("/blort-out");
    PortEntry * blorgIn = findPort("/blorg");
    
    if (blortOut && blorgIn)
    {
        blortOut->addOutputConnection(blorgIn);
        blorgIn->addInputConnection(blortOut);
    }
    PortEntry * blirgOut = findPort("/blirg");
    PortEntry * blortIn = findPort("/blort-in");
    
    if (blirgOut && blortIn)
    {
        blirgOut->addOutputConnection(blortIn);
        blortIn->addInputConnection(blirgOut);
    }
#else // ! defined(TEST_GRAPHICS_)
    MplusM::Common::StringVector services;
    
    MplusM::Utilities::GetServiceNames(services, true);
    if (services.size())
    {
        for (size_t ii = 0, mm = services.size(); mm > ii; ++ii)
        {
            yarp::os::ConstString aService = services[ii];
            
            MplusM::Utilities::ServiceDescriptor descriptor;
            
            if (MplusM::Utilities::GetNameAndDescriptionForService(aService, descriptor))
            {
                ServiceEntity * anEntity = new ServiceEntity(this);
                
                anEntity->setup(descriptor._canonicalName);
                anEntity->addPort(aService, PortEntry::kPortDirectionInput);
                for (int jj = 0, nn = descriptor._inputChannels.size(); nn > jj; ++jj)
                {
                    anEntity->addPort(descriptor._inputChannels[jj], PortEntry::kPortDirectionInput);
                }
                for (int jj = 0, nn = descriptor._outputChannels.size(); nn > jj; ++jj)
                {
                    anEntity->addPort(descriptor._outputChannels[jj], PortEntry::kPortDirectionOutput);
                }
                _entities.push_back(anEntity);
            }
        }
    }
    MplusM::Utilities::PortVector detectedPorts;
    
    MplusM::Utilities::GetDetectedPortList(detectedPorts);
    if (detectedPorts.size())
    {
        size_t mm = detectedPorts.size();
        
        for (size_t ii = 0; mm > ii; ++ii)
        {
            const MplusM::Utilities::PortDescriptor & aDescriptor = detectedPorts[ii];
            
#if 0
            cout << aDescriptor._portName.c_str() << " " << aDescriptor._portIpAddress.c_str() << " " <<
                    aDescriptor._portPortNumber.c_str() << endl;
#endif//0
            if (! findPort(aDescriptor._portName.c_str()))
            {
                ServiceEntity * anEntity = new ServiceEntity(this);
                
                anEntity->setup((aDescriptor._portIpAddress + ":" + aDescriptor._portPortNumber).c_str());
                anEntity->addPort(aDescriptor._portName, PortEntry::kPortDirectionInputOutput);
                _entities.push_back(anEntity);
            }
        }
        // Adde the connections
        for (size_t ii = 0; mm > ii; ++ii)
        {
            const MplusM::Utilities::PortDescriptor & aDescriptor = detectedPorts[ii];
            MplusM::Common::StringVector              inputs;
            MplusM::Common::StringVector              outputs;
            PortEntry *                               thisPort = findPort(aDescriptor._portName);
            
            if (thisPort)
            {
                MplusM::Utilities::GatherPortConnections(aDescriptor._portName, inputs, outputs,
                                                         MplusM::Utilities::kInputAndOutputOutput, true);
                for (int jj = 0, mm = outputs.size(); mm > jj; ++jj)
                {
                    PortEntry * otherPort = findPort(outputs[jj]);
                    
                    if (otherPort)
                    {
                        thisPort->addOutputConnection(otherPort);
                        otherPort->addInputConnection(thisPort);
                    }
                }
            }
        }
    }
#endif // ! defined(TEST_GRAPHICS_)
} // ServiceViewerApp::gatherEntities

void ServiceViewerApp::gotMessage(ofMessage msg)
{
    inherited::gotMessage(msg);
} // ServiceViewerApp::gotMessage

void ServiceViewerApp::keyPressed(int key)
{
    if (OF_KEY_ALT == (key & OF_KEY_ALT))
    {
        _altActive = true;
    }
    if (OF_KEY_CONTROL == (key & OF_KEY_COMMAND))
    {
        _commandActive = true;
    }
    if (OF_KEY_CONTROL == (key & OF_KEY_CONTROL))
    {
        _controlActive = true;
    }
    if (OF_KEY_SHIFT == (key & OF_KEY_SHIFT))
    {
        _shiftActive = true;
    }
    inherited::keyPressed(key);
} // ServiceViewerApp::keyPressed

void ServiceViewerApp::keyReleased(int key)
{
    if (OF_KEY_ALT == (key & OF_KEY_ALT))
    {
        _altActive = false;
    }
    if (OF_KEY_CONTROL == (key & OF_KEY_COMMAND))
    {
        _commandActive = false;
    }
    if (OF_KEY_CONTROL == (key & OF_KEY_CONTROL))
    {
        _controlActive = false;
    }
    if (OF_KEY_SHIFT == (key & OF_KEY_SHIFT))
    {
        _shiftActive = false;
    }
    inherited::keyReleased(key);
} // ServiceViewerApp::keyReleased

void ServiceViewerApp::mouseDragged(int x,
                                    int y,
                                    int button)
{
    inherited::mouseDragged(x, y, button);
} // ServiceViewerApp::mouseDragged

void ServiceViewerApp::mouseMoved(int x,
                                  int y)
{
    inherited::mouseMoved(x, y);
} // ServiceViewerApp::mouseMoved

void ServiceViewerApp::mousePressed(int x,
                                    int y,
                                    int button)
{
//    cout << "pressed here" << endl;
    inherited::mousePressed(x, y, button);
} // ServiceViewerApp::mousePressed

void ServiceViewerApp::mouseReleased(int x,
                                     int y,
                                     int button)
{
    inherited::mouseReleased(x, y, button);
} // ServiceViewerApp::mouseReleased

void ServiceViewerApp::rememberPort(PortEntry * aPort)
{
    if (aPort)
    {
        _ports.insert(PortMap::value_type(static_cast<string>(*aPort), aPort));
    }
} // ServiceViewerApp::rememberPort

void ServiceViewerApp::setInitialEntityPositions(void)
{
//#if defined(TEST_GRAPHICS_)
    float fullHeight = ofGetHeight();
    float fullWidth = ofGetWidth();
    
    for (EntityList::iterator it(_entities.begin()); _entities.end() != it; ++it)
    {
        ServiceEntity * anEntity = *it;

        if (anEntity)
        {
            ofRectangle entityShape = anEntity->getShape();
            float       newX = ofRandom(fullWidth - entityShape.width);
            float       newY = ofRandom(fullHeight - entityShape.height);
            
            anEntity->setPosition(newX, newY);
        }
    }
//#else // ! defined(TEST_GRAPHICS_)
//#endif // ! defined(TEST_GRAPHICS_)
} // ServiceViewerApp::setInitialEntityPositions

void ServiceViewerApp::setup(void)
{
    ofSetWindowTitle("Service Viewer");
#if CheckNetworkWorks_
    if (yarp::os::Network::checkNetwork())
#endif // CheckNetworkWorks_
    {
        _networkAvailable = true;
        ofSetLogLevel(OF_LOG_VERBOSE);
        gatherEntities();
        setInitialEntityPositions();
    }
#if CheckNetworkWorks_
    else
    {
        OD_LOG("! (yarp::os::Network::checkNetwork())");//####
        cerr << "YARP network not running." << endl;
        _networkAvailable = false;
    }
#endif // CheckNetworkWorks_
} // ServiceViewerApp::setup

void ServiceViewerApp::update(void)
{
    inherited::update();
} // ServiceViewerApp::update

void ServiceViewerApp::updateEntityList(ServiceEntity * anEntity)
{
    if (anEntity->isSelected())
    {
        EntityList::iterator it(_entities.begin());
        
        for ( ; _entities.end() != it; ++it)
        {
            ServiceEntity * anEntity = *it;
            
            if (anEntity && anEntity->isSelected())
            {
                _entities.erase(it);
                _entities.push_back(anEntity);
                break;
            }
            
        }
    }
} // ServiceViewerApp::updateEntityList

void ServiceViewerApp::windowResized(int w,
                                     int h)
{
    inherited::windowResized(w, h);
} // ServiceViewerApp::windowResized

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)