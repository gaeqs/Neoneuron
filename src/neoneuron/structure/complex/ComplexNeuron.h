//
// Created by gaeqs on 8/10/24.
//

#ifndef COMPLEXNEURON_H
#define COMPLEXNEURON_H

#include <vector>
#include <unordered_map>
#include <mindset/Dataset.h>
#include <mindset/Neuron.h>

#include <neoneuron/structure/complex/ComplexNeuronSegment.h>

#include "ComplexJoint.h"
#include "ComplexSoma.h"

namespace neoneuron
{
    class ComplexNeuron : public mindset::Identifiable
    {
        mindset::Dataset* _dataset;
        std::vector<ComplexNeuronSegment> _segments;
        std::unordered_map<mindset::UID, size_t> _segmentsByUID;
        std::vector<ComplexJoint> _joints;
        std::unordered_map<mindset::UID, size_t> _jointsByUID;
        std::vector<ComplexSoma> _somas;
        std::unordered_map<mindset::UID, size_t> _somasByUID;
        rush::AABB<3, float> _boundingBox;

        void calculateBoundingBox();

        void calculateJoints();

        void calculateSomas();

        void calculateLOD();

        void calculateLODFrom(ComplexNeuronSegment* segment);

      public:
        ComplexNeuron(const ComplexNeuron& other) = delete;

        ComplexNeuron(ComplexNeuron&& other) noexcept;

        ComplexNeuron& operator=(ComplexNeuron&& other) noexcept;

        ComplexNeuron(mindset::Dataset* dataset, mindset::Neuron* prototype);

        mindset::Neuron* getPrototypeNeuron();

        const mindset::Neuron* getPrototypeNeuron() const;

        mindset::Dataset* getDataset();

        const mindset::Dataset* getDataset() const;

        [[nodiscard]] rush::AABB<3, float> getBoundingBox() const;

        [[nodiscard]] const std::vector<ComplexNeuronSegment>& getSegments() const;

        [[nodiscard]] std::optional<const ComplexNeuronSegment*> findSegment(mindset::UID uid) const;

        [[nodiscard]] std::optional<size_t> findSegmentIndex(mindset::UID uid) const;

        [[nodiscard]] const std::vector<ComplexJoint>& getJoints() const;

        [[nodiscard]] std::optional<const ComplexJoint*> findJoint(mindset::UID uid) const;

        [[nodiscard]] std::optional<size_t> findJointIndex(mindset::UID uid) const;

        [[nodiscard]] const std::vector<ComplexSoma>& getSomas() const;

        [[nodiscard]] std::optional<const ComplexSoma*> findSoma(mindset::UID uid) const;

        [[nodiscard]] std::optional<size_t> findSomaIndex(mindset::UID uid) const;

        void recalculateMetadata();

        void refreshProperty(const std::string& propertyName);
    };
} // namespace neoneuron

#endif //COMPLEXNEURON_H
