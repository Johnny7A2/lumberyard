/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once

#include <GraphCanvas/Components/NodePropertyDisplay/VectorDataInterface.h>

#include "ScriptCanvasDataInterface.h"

namespace ScriptCanvasEditor
{
    template<class Type, int ElementCount>
    class ScriptCanvasVectorizedDataInterface
        : public ScriptCanvasDataInterface<GraphCanvas::VectorDataInterface>
    {
    public:
        AZ_CLASS_ALLOCATOR(ScriptCanvasVectorizedDataInterface, AZ::SystemAllocator, 0);
        ScriptCanvasVectorizedDataInterface(const AZ::EntityId& nodeId, const ScriptCanvas::SlotId& slotId)
            : ScriptCanvasDataInterface(nodeId, slotId)
        {
        }
        
        ~ScriptCanvasVectorizedDataInterface() = default;
        
        int GetElementCount() const override
        {
            return ElementCount;
        }
        
        double GetValue(int index) const override
        {
            if (index < GetElementCount())
            {
                if (const ScriptCanvas::Datum* object = GetSlotObject())
                {
                    if (const Type* retVal = object->GetAs<Type>())
                    {
                        return aznumeric_cast<double>(static_cast<float>(retVal->GetElement(index)));
                    }
                }
            }
            
            return 0.0;
        }

        void SetValue(int index, double value) override
        {
            if (index < GetElementCount())
            {
                if (ScriptCanvas::Datum* object = GetSlotObject())
                {
                    if (Type* currentType = object->ModAs<Type>())
                    {
                        currentType->SetElement(index, aznumeric_cast<float>(value));

                        PostUndoPoint();
                        PropertyGridRequestBus::Broadcast(&PropertyGridRequests::RefreshPropertyGrid);
                    }
                }
            }
        }
    };

    template<class Type, int ElementCount>
    class ScriptCanvasVectorDataInterface
        : public ScriptCanvasVectorizedDataInterface<Type, ElementCount>
    {
    public:
        AZ_CLASS_ALLOCATOR(ScriptCanvasVectorDataInterface, AZ::SystemAllocator, 0);
        ScriptCanvasVectorDataInterface(const AZ::EntityId& nodeId, const ScriptCanvas::SlotId& slotId)
            : ScriptCanvasVectorizedDataInterface<Type, ElementCount>(nodeId, slotId)
        {
        }

        ~ScriptCanvasVectorDataInterface() = default;

        const char* GetLabel(int index) const override
        {
            if (index == 0)
            {
                return "X";
            }
            else if (index == 1)
            {
                return "Y";
            }
            else if (index == 2)
            {
                return "Z";
            }
            else if (index == 3)
            {
                return "W";
            }

            return "???";
        }

        AZStd::string GetStyle() const
        {
            return "vectorized";
        }

        AZStd::string GetElementStyle(int index) const override
        {
            return AZStd::string::format("vector_%i", index);
        }
    };
}