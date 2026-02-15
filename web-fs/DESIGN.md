# PulseNet - AI-Driven Rural Healthcare System
## Design Document

---

## 1. System Overview

### Vision
PulseNet is a cloud-native, AI-powered healthcare platform designed to bridge the healthcare gap in rural and underserved areas. The system provides intelligent symptom analysis, treatment recommendations, real-time inventory management, and emergency response coordination.

### High-Level Architecture
PulseNet follows a modern microservices architecture built on AWS cloud infrastructure, leveraging serverless computing, managed AI/ML services, and event-driven patterns to ensure scalability, reliability, and cost-effectiveness.

```
┌─────────────────────────────────────────────────────────────────┐
│                        Frontend Layer                            │
│  (Mobile/Web Apps - Flutter/React Native - Offline-First)       │
└────────────────────────┬────────────────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────────────────┐
│                     API Gateway Layer                            │
│              (AWS API Gateway + Authentication)                  │
└────────────────────────┬────────────────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────────────────┐
│                    Backend Services Layer                        │
│     (AWS Lambda + FastAPI/Node.js Microservices)                │
└─────┬──────────────────┬──────────────────┬─────────────────────┘
      │                  │                  │
┌─────▼─────┐   ┌───────▼────────┐   ┌────▼──────────────────────┐
│  AI/ML    │   │  Data Layer    │   │  Integration & Events     │
│  Layer    │   │  (RDS/DynamoDB)│   │  (EventBridge/SNS/SQS)   │
└───────────┘   └────────────────┘   └───────────────────────────┘
```

### Core Design Principles

1. **Scalability**: Serverless architecture with auto-scaling capabilities
2. **Reliability**: Multi-AZ deployment, automated failover, 99.9% uptime SLA
3. **Real-Time Processing**: Event-driven architecture for instant alerts
4. **Offline-First**: Local data caching for low-connectivity environments
5. **Security**: End-to-end encryption, HIPAA compliance, zero-trust architecture
6. **Cost-Efficiency**: Pay-per-use serverless model, optimized for rural deployment
7. **Resilience**: Disaster mode with automated surge detection and response

---

## 2. Architecture Components

### 2.1 Frontend Layer

#### Technology Stack
- **Framework**: Flutter (mobile) / React Native (cross-platform)
- **Web**: React.js with Progressive Web App (PWA) capabilities
- **State Management**: Redux/Provider for consistent state handling
- **Offline Storage**: SQLite/Hive for local data persistence

#### User Interfaces

**Patient Portal**
- Symptom reporting with guided questionnaire
- Treatment plan viewing and medication reminders
- Appointment scheduling
- Health history tracking
- Emergency SOS button

**Doctor Dashboard**
- Patient queue management
- AI-assisted diagnosis review
- Treatment plan approval/modification
- Prescription generation
- Telemedicine video consultation

**Supplier Portal**
- Inventory alerts and notifications
- Order management
- Delivery tracking
- Demand forecasting insights

**Government Official Dashboard**
- Regional health analytics
- Disease outbreak monitoring
- Resource allocation insights
- Emergency response coordination

#### Offline-First Design
- Local SQLite database syncs with cloud when connectivity available
- Queue-based sync mechanism with conflict resolution
- Critical operations cached locally (symptom reports, prescriptions)
- Progressive data loading with skeleton screens
- Automatic retry with exponential backoff

---

### 2.2 Backend Layer

#### AWS API Gateway
- RESTful API endpoints for all services
- WebSocket API for real-time notifications
- Request throttling and rate limiting
- API versioning (v1, v2)
- CORS configuration for web clients
- Request/response transformation

#### Microservices Architecture

**Patient Service** (AWS Lambda + FastAPI)
- Patient registration and profile management
- Medical history CRUD operations
- Symptom report submission
- Treatment plan retrieval

**Diagnosis Service** (AWS Lambda + Node.js)
- AI model invocation for symptom analysis
- Treatment recommendation generation
- Doctor review workflow
- Prescription generation

**Inventory Service** (AWS Lambda + FastAPI)
- Real-time stock tracking
- Depletion prediction
- Reorder point calculation
- Supplier notification triggers

**Alert Service** (AWS Lambda + Node.js)
- Emergency alert generation
- Notification routing (SMS, push, email)
- Alert escalation logic
- Disaster mode activation

**Analytics Service** (AWS Lambda + Python)
- Health metrics aggregation
- Disease pattern detection
- Resource utilization reports
- Predictive analytics

**Authentication Service** (AWS Lambda + Node.js)
- JWT token generation and validation
- Role-based access control (RBAC)
- Multi-factor authentication (MFA)
- Session management

#### Lambda Configuration
- Runtime: Python 3.11 (FastAPI), Node.js 20.x
- Memory: 512MB - 2GB based on service
- Timeout: 30s (API), 5min (batch processing)
- Concurrency: Reserved capacity for critical services
- VPC integration for database access

---

### 2.3 AI/ML Layer

#### AWS SageMaker Infrastructure

**Model Training Pipeline**
- SageMaker Training Jobs with GPU instances (ml.p3.2xlarge)
- Automated hyperparameter tuning
- Model versioning with SageMaker Model Registry
- A/B testing for model deployment
- Continuous retraining with new data

**Model Deployment**
- SageMaker Real-time Endpoints for low-latency inference
- Serverless Inference for variable workloads
- Multi-model endpoints for cost optimization
- Auto-scaling based on invocation metrics

#### ML Models

**Symptom Analysis Model**
- Architecture: Transformer-based NLP (BERT/BioBERT)
- Input: Patient symptoms, demographics, medical history
- Output: Disease probability distribution, confidence scores
- Training Data: Anonymized medical records, symptom databases
- Accuracy Target: >92% for common conditions

**Treatment Recommendation Engine**
- Architecture: Gradient Boosting (XGBoost) + Rule-based system
- Input: Diagnosis, patient profile, drug interactions, local availability
- Output: Ranked treatment options with dosage recommendations
- Features: Age, weight, allergies, comorbidities, drug stock levels

**Inventory Prediction Model**
- Architecture: LSTM (Long Short-Term Memory) neural network
- Input: Historical consumption, seasonal patterns, disease outbreaks
- Output: 7-day, 30-day demand forecasts per medication
- Features: Time-series data, regional health trends, weather patterns
- Retraining: Weekly with latest consumption data

**Surge Detection Model**
- Architecture: Statistical anomaly detection (Z-score, IQR) + ML classifier
- Input: Patient visit rates, symptom patterns, geographic clustering
- Output: Outbreak probability, severity level (1-5)
- Threshold: 2.5 standard deviations above baseline
- Real-time processing with 5-minute windows

#### Data Storage

**Amazon S3 Buckets**
- `pulsenet-training-data`: Anonymized datasets for model training
- `pulsenet-models`: Trained model artifacts and checkpoints
- `pulsenet-inference-logs`: Prediction logs for monitoring
- Lifecycle policies: Archive to Glacier after 90 days
- Versioning enabled for model artifacts

**SageMaker Feature Store**
- Patient feature groups (demographics, history)
- Inventory feature groups (stock levels, consumption rates)
- Real-time and batch feature serving
- Point-in-time correct queries for training

---

### 2.4 Data Layer

#### Amazon RDS (PostgreSQL 15)

**Configuration**
- Instance: db.r6g.xlarge (Multi-AZ deployment)
- Storage: 500GB GP3 with auto-scaling to 2TB
- Backup: Automated daily snapshots, 7-day retention
- Read Replicas: 2 replicas for read-heavy operations
- Connection Pooling: RDS Proxy for Lambda connections

**Database Schema**

```sql
-- Patients Table
CREATE TABLE patients (
    patient_id UUID PRIMARY KEY,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    date_of_birth DATE NOT NULL,
    gender VARCHAR(20),
    phone_number VARCHAR(20) UNIQUE,
    address TEXT,
    village_id UUID REFERENCES villages(village_id),
    created_at TIMESTAMP DEFAULT NOW(),
    updated_at TIMESTAMP DEFAULT NOW()
);
CREATE INDEX idx_patients_village ON patients(village_id);
CREATE INDEX idx_patients_phone ON patients(phone_number);

-- Medical History Table
CREATE TABLE medical_history (
    history_id UUID PRIMARY KEY,
    patient_id UUID REFERENCES patients(patient_id),
    condition VARCHAR(200),
    diagnosed_date DATE,
    status VARCHAR(50), -- active, resolved, chronic
    notes TEXT,
    created_at TIMESTAMP DEFAULT NOW()
);
CREATE INDEX idx_history_patient ON medical_history(patient_id);

-- Symptom Reports Table
CREATE TABLE symptom_reports (
    report_id UUID PRIMARY KEY,
    patient_id UUID REFERENCES patients(patient_id),
    symptoms JSONB NOT NULL, -- {fever: true, cough: "severe", duration_days: 3}
    severity VARCHAR(20), -- mild, moderate, severe, critical
    reported_at TIMESTAMP DEFAULT NOW(),
    location GEOGRAPHY(POINT), -- PostGIS for geospatial queries
    status VARCHAR(50) DEFAULT 'pending' -- pending, analyzed, treated
);
CREATE INDEX idx_reports_patient ON symptom_reports(patient_id);
CREATE INDEX idx_reports_status ON symptom_reports(status);
CREATE INDEX idx_reports_location ON symptom_reports USING GIST(location);

-- Diagnoses Table
CREATE TABLE diagnoses (
    diagnosis_id UUID PRIMARY KEY,
    report_id UUID REFERENCES symptom_reports(report_id),
    ai_diagnosis JSONB, -- {disease: "malaria", confidence: 0.89, alternatives: [...]}
    doctor_diagnosis VARCHAR(200),
    doctor_id UUID REFERENCES doctors(doctor_id),
    reviewed_at TIMESTAMP,
    final_diagnosis VARCHAR(200),
    created_at TIMESTAMP DEFAULT NOW()
);

-- Treatment Plans Table
CREATE TABLE treatment_plans (
    plan_id UUID PRIMARY KEY,
    diagnosis_id UUID REFERENCES diagnoses(diagnosis_id),
    patient_id UUID REFERENCES patients(patient_id),
    medications JSONB, -- [{drug: "paracetamol", dosage: "500mg", frequency: "3x daily", duration: "5 days"}]
    instructions TEXT,
    follow_up_date DATE,
    status VARCHAR(50), -- active, completed, discontinued
    created_by UUID REFERENCES doctors(doctor_id),
    created_at TIMESTAMP DEFAULT NOW()
);

-- Prescriptions Table
CREATE TABLE prescriptions (
    prescription_id UUID PRIMARY KEY,
    plan_id UUID REFERENCES treatment_plans(plan_id),
    patient_id UUID REFERENCES patients(patient_id),
    prescribed_by UUID REFERENCES doctors(doctor_id),
    medications JSONB,
    dispensed BOOLEAN DEFAULT FALSE,
    dispensed_at TIMESTAMP,
    dispensed_by UUID,
    created_at TIMESTAMP DEFAULT NOW()
);

-- Inventory Table
CREATE TABLE inventory (
    inventory_id UUID PRIMARY KEY,
    facility_id UUID REFERENCES facilities(facility_id),
    medication_name VARCHAR(200) NOT NULL,
    medication_type VARCHAR(100), -- tablet, syrup, injection
    quantity INTEGER NOT NULL,
    unit VARCHAR(50), -- tablets, ml, vials
    expiry_date DATE,
    reorder_point INTEGER,
    last_updated TIMESTAMP DEFAULT NOW()
);
CREATE INDEX idx_inventory_facility ON inventory(facility_id);
CREATE INDEX idx_inventory_medication ON inventory(medication_name);

-- Suppliers Table
CREATE TABLE suppliers (
    supplier_id UUID PRIMARY KEY,
    name VARCHAR(200) NOT NULL,
    contact_person VARCHAR(100),
    phone_number VARCHAR(20),
    email VARCHAR(100),
    address TEXT,
    medications_supplied JSONB, -- [medication names]
    rating DECIMAL(3,2),
    created_at TIMESTAMP DEFAULT NOW()
);

-- Alerts Table
CREATE TABLE alerts (
    alert_id UUID PRIMARY KEY,
    alert_type VARCHAR(50), -- inventory_low, outbreak_detected, emergency
    severity VARCHAR(20), -- info, warning, critical
    title VARCHAR(200),
    message TEXT,
    metadata JSONB,
    target_roles JSONB, -- ["doctor", "supplier", "government"]
    facility_id UUID REFERENCES facilities(facility_id),
    resolved BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT NOW(),
    resolved_at TIMESTAMP
);
CREATE INDEX idx_alerts_type ON alerts(alert_type);
CREATE INDEX idx_alerts_severity ON alerts(severity);
CREATE INDEX idx_alerts_resolved ON alerts(resolved);
```

#### Amazon Timestream

**Time-Series Data for Inventory Tracking**

```
Database: PulseNetInventory
Table: MedicationConsumption

Schema:
- time: TIMESTAMP (partition key)
- facility_id: VARCHAR (dimension)
- medication_name: VARCHAR (dimension)
- quantity_consumed: BIGINT (measure)
- quantity_remaining: BIGINT (measure)
- consumption_rate: DOUBLE (measure) -- units per day
- predicted_stockout_date: TIMESTAMP (measure)
```

**Benefits**
- Optimized for time-series queries (consumption trends, forecasting)
- Automatic data lifecycle management (hot/warm/cold storage)
- Cost-effective: 1/10th cost of relational databases for time-series
- Built-in interpolation and aggregation functions

**Retention Policy**
- Memory store: 7 days (fast queries)
- Magnetic store: 2 years (historical analysis)
- Auto-archival to S3 after 2 years

#### DynamoDB (Alternative for High-Scale Scenarios)

**Use Case**: If RDS becomes a bottleneck for read-heavy operations

**Tables**
- `Patients`: Partition key: patient_id
- `SymptomReports`: Partition key: patient_id, Sort key: reported_at
- `Inventory`: Partition key: facility_id, Sort key: medication_name
- Global Secondary Indexes for query patterns

**Advantages**
- Single-digit millisecond latency
- Unlimited scaling
- On-demand pricing for variable workloads

---

### 2.5 Integration & Event Layer

#### Amazon EventBridge

**Event Bus Architecture**

```
Custom Event Bus: PulseNetEvents

Event Patterns:
1. InventoryDepletionEvent
   - Source: inventory-service
   - DetailType: inventory.low
   - Targets: alert-service, supplier-notification-lambda

2. DiagnosisCompletedEvent
   - Source: diagnosis-service
   - DetailType: diagnosis.completed
   - Targets: treatment-service, patient-notification-lambda

3. SurgeDetectedEvent
   - Source: analytics-service
   - DetailType: outbreak.detected
   - Targets: disaster-mode-step-function, government-alert-lambda

4. PrescriptionDispensedEvent
   - Source: pharmacy-service
   - DetailType: prescription.dispensed
   - Targets: inventory-update-lambda, analytics-service
```

**Benefits**
- Decoupled microservices communication
- Event replay for debugging
- Schema registry for event validation
- Cross-account event routing for multi-region

#### Amazon SNS (Simple Notification Service)

**Topics**

```
1. CriticalAlerts
   - Subscribers: SMS (doctors, government), Email, Lambda (logging)
   - Filter policies: severity = "critical"

2. InventoryAlerts
   - Subscribers: Email (suppliers), SMS (facility managers)
   - Filter policies: alert_type = "inventory_low"

3. PatientNotifications
   - Subscribers: Mobile push (via Lambda → Firebase), SMS
   - Filter policies: patient_id attribute matching
```

**Message Format**
```json
{
  "alertType": "inventory_low",
  "severity": "warning",
  "facilityId": "fac-123",
  "medication": "Paracetamol 500mg",
  "currentStock": 50,
  "reorderPoint": 200,
  "estimatedStockoutDate": "2026-02-20",
  "timestamp": "2026-02-16T10:30:00Z"
}
```

#### Amazon SQS (Simple Queue Service)

**Queues**

```
1. SymptomAnalysisQueue (Standard Queue)
   - Purpose: Buffer symptom reports for AI processing
   - Visibility timeout: 5 minutes
   - Dead-letter queue: SymptomAnalysisDLQ
   - Consumer: diagnosis-service Lambda

2. NotificationQueue (FIFO Queue)
   - Purpose: Ordered notification delivery
   - Message deduplication: Enabled
   - Consumer: notification-service Lambda

3. InventoryUpdateQueue (Standard Queue)
   - Purpose: Batch inventory updates
   - Batch size: 10 messages
   - Consumer: inventory-service Lambda
```

#### Apache Kafka (Optional for High-Throughput Scenarios)

**Use Case**: Real-time streaming analytics for large-scale deployments

**Topics**
- `patient-events`: All patient interactions
- `inventory-events`: Stock movements
- `health-metrics`: Aggregated health data

**Consumers**
- Real-time analytics dashboard
- ML model feature engineering
- Audit logging system

**Deployment**: Amazon MSK (Managed Streaming for Kafka)

#### Firebase Cloud Messaging / Twilio

**Firebase (Mobile Push Notifications)**
- Integration via Lambda function
- Topic-based messaging for broadcast alerts
- Device token management in DynamoDB
- Notification payload with deep links

**Twilio (SMS Notifications)**
- SMS gateway for critical alerts
- Fallback when internet unavailable
- Two-way SMS for appointment confirmations
- Cost optimization: SMS only for critical alerts

---

### 2.6 Monitoring & Emergency Response

#### Amazon CloudWatch

**Metrics**

```
Custom Metrics:
- SymptomReportsPerMinute (by region)
- AIModelInferenceLatency (p50, p99)
- InventoryStockLevels (by medication)
- APIGatewayErrorRate (by endpoint)
- LambdaConcurrentExecutions (by function)
- DatabaseConnectionPoolUtilization
- SurgeDetectionScore (real-time)
```

**Logs**
- Lambda function logs (structured JSON)
- API Gateway access logs
- Application logs from all services
- Audit logs for compliance

**Log Insights Queries**
```
# Detect symptom patterns
fields @timestamp, symptoms, location
| filter status = "pending"
| stats count() by symptoms.fever, symptoms.cough
| sort count desc

# Track inventory depletion rate
fields @timestamp, medication_name, quantity_consumed
| stats sum(quantity_consumed) by medication_name, bin(5m)
```

**Alarms**

```
1. HighSymptomReportRate
   - Metric: SymptomReportsPerMinute > 100 (3 consecutive periods)
   - Action: Trigger surge detection Lambda, SNS notification

2. LowInventoryAlert
   - Metric: InventoryStockLevels < ReorderPoint
   - Action: SNS to suppliers, EventBridge event

3. APIGatewayHighErrorRate
   - Metric: 5XXError > 5% (2 consecutive periods)
   - Action: SNS to DevOps team, auto-scaling trigger

4. DatabaseHighCPU
   - Metric: RDS CPU > 80% (5 minutes)
   - Action: Scale up instance, alert DBA

5. LambdaThrottling
   - Metric: Throttles > 10 (1 minute)
   - Action: Increase reserved concurrency
```

**Dashboards**

```
1. Operations Dashboard
   - API request rates and latencies
   - Lambda execution metrics
   - Database performance
   - Error rates by service

2. Health Analytics Dashboard (Grafana)
   - Disease prevalence by region
   - Patient visit trends
   - Treatment outcomes
   - Inventory consumption patterns

3. Emergency Response Dashboard
   - Real-time surge detection score
   - Active outbreak locations (map view)
   - Hospital capacity utilization
   - Emergency alert status
```

#### Grafana Integration

**Data Sources**
- Amazon Timestream (inventory time-series)
- CloudWatch Metrics
- RDS PostgreSQL (aggregated health data)

**Dashboards**

```
Inventory Management Dashboard:
- Stock levels by medication (gauge charts)
- Consumption trends (line charts)
- Predicted stockout dates (table)
- Supplier delivery performance (bar charts)

Health Trends Dashboard:
- Disease prevalence heatmap
- Symptom frequency word cloud
- Treatment success rates (pie charts)
- Patient demographics (bar charts)
```

#### AWS Step Functions - Disaster Mode Orchestration

**State Machine: DisasterResponseWorkflow**

```json
{
  "Comment": "Orchestrates emergency response during disease outbreaks",
  "StartAt": "ValidateSurge",
  "States": {
    "ValidateSurge": {
      "Type": "Task",
      "Resource": "arn:aws:lambda:region:account:function:validate-surge",
      "Next": "IsCritical"
    },
    "IsCritical": {
      "Type": "Choice",
      "Choices": [
        {
          "Variable": "$.severity",
          "NumericGreaterThanEquals": 4,
          "Next": "ActivateDisasterMode"
        }
      ],
      "Default": "SendWarningAlerts"
    },
    "ActivateDisasterMode": {
      "Type": "Parallel",
      "Branches": [
        {
          "StartAt": "NotifyGovernment",
          "States": {
            "NotifyGovernment": {
              "Type": "Task",
              "Resource": "arn:aws:lambda:region:account:function:notify-government",
              "End": true
            }
          }
        },
        {
          "StartAt": "AlertHospitals",
          "States": {
            "AlertHospitals": {
              "Type": "Task",
              "Resource": "arn:aws:lambda:region:account:function:alert-hospitals",
              "End": true
            }
          }
        },
        {
          "StartAt": "RequestEmergencySupplies",
          "States": {
            "RequestEmergencySupplies": {
              "Type": "Task",
              "Resource": "arn:aws:lambda:region:account:function:request-supplies",
              "End": true
            }
          }
        },
        {
          "StartAt": "DeployMobileUnits",
          "States": {
            "DeployMobileUnits": {
              "Type": "Task",
              "Resource": "arn:aws:lambda:region:account:function:deploy-mobile-units",
              "End": true
            }
          }
        }
      ],
      "Next": "MonitorSituation"
    },
    "MonitorSituation": {
      "Type": "Wait",
      "Seconds": 3600,
      "Next": "CheckResolution"
    },
    "CheckResolution": {
      "Type": "Task",
      "Resource": "arn:aws:lambda:region:account:function:check-resolution",
      "Next": "IsResolved"
    },
    "IsResolved": {
      "Type": "Choice",
      "Choices": [
        {
          "Variable": "$.resolved",
          "BooleanEquals": true,
          "Next": "DeactivateDisasterMode"
        }
      ],
      "Default": "MonitorSituation"
    },
    "DeactivateDisasterMode": {
      "Type": "Task",
      "Resource": "arn:aws:lambda:region:account:function:deactivate-disaster-mode",
      "End": true
    },
    "SendWarningAlerts": {
      "Type": "Task",
      "Resource": "arn:aws:lambda:region:account:function:send-warning",
      "End": true
    }
  }
}
```

#### Surge Detection Algorithm

**Statistical Approach**

```python
def detect_surge(current_rate, historical_data, window_minutes=60):
    """
    Detects disease outbreak surge using statistical thresholds
    
    Args:
        current_rate: Current symptom reports per minute
        historical_data: Past 30 days of symptom report rates
        window_minutes: Time window for rate calculation
    
    Returns:
        {
            "surge_detected": bool,
            "severity": int (1-5),
            "confidence": float,
            "baseline": float,
            "z_score": float
        }
    """
    # Calculate baseline (mean of historical data)
    baseline = np.mean(historical_data)
    std_dev = np.std(historical_data)
    
    # Calculate Z-score
    z_score = (current_rate - baseline) / std_dev
    
    # Detect surge (2.5 standard deviations above baseline)
    surge_detected = z_score > 2.5
    
    # Calculate severity (1-5 scale)
    if z_score > 5:
        severity = 5  # Critical
    elif z_score > 4:
        severity = 4  # Severe
    elif z_score > 3:
        severity = 3  # Moderate
    elif z_score > 2.5:
        severity = 2  # Mild
    else:
        severity = 1  # Normal
    
    # Confidence based on data quality
    confidence = min(1.0, len(historical_data) / (30 * 24 * 60))
    
    return {
        "surge_detected": surge_detected,
        "severity": severity,
        "confidence": confidence,
        "baseline": baseline,
        "z_score": z_score,
        "current_rate": current_rate
    }
```

**Geospatial Clustering**
- Use PostGIS to detect geographic clusters of similar symptoms
- DBSCAN algorithm for outbreak hotspot identification
- Trigger alerts when cluster density exceeds threshold

---

## 3. Data Flow Diagrams

### 3.1 Patient Symptom Reporting → Treatment Plan

```
┌─────────────┐
│   Patient   │
│  Mobile App │
└──────┬──────┘
       │ 1. Submit symptoms
       ▼
┌─────────────────┐
│  API Gateway    │
└──────┬──────────┘
       │ 2. Authenticate & validate
       ▼
┌─────────────────┐
│ Patient Service │ ──────► 3. Store in RDS
│   (Lambda)      │
└──────┬──────────┘
       │ 4. Publish SymptomReportedEvent
       ▼
┌─────────────────┐
│  EventBridge    │
└──────┬──────────┘
       │ 5. Route to diagnosis service
       ▼
┌─────────────────┐
│Diagnosis Service│
│   (Lambda)      │
└──────┬──────────┘
       │ 6. Invoke SageMaker endpoint
       ▼
┌─────────────────┐
│  SageMaker      │
│ Symptom Model   │ ──────► 7. Return diagnosis + confidence
└──────┬──────────┘
       │
       ▼
┌─────────────────┐
│Treatment Service│ ──────► 8. Generate treatment plan
│   (Lambda)      │         9. Check inventory availability
└──────┬──────────┘        10. Store in RDS
       │
       ▼
┌─────────────────┐
│ Notification    │ ──────► 11. Notify patient (Firebase/SMS)
│   Service       │         12. Notify doctor for review
└─────────────────┘
```

### 3.2 Inventory Depletion → Supplier Alert

```
┌─────────────────┐
│ Pharmacy Staff  │
│  Dispenses Rx   │
└──────┬──────────┘
       │ 1. Record dispensation
       ▼
┌─────────────────┐
│Inventory Service│ ──────► 2. Update stock in RDS
│   (Lambda)      │         3. Write to Timestream
└──────┬──────────┘
       │ 4. Check reorder point
       ▼
┌─────────────────┐
│  SageMaker      │ ──────► 5. Predict future demand
│Inventory Model  │         6. Calculate stockout date
└──────┬──────────┘
       │
       ▼
┌─────────────────┐
│ Inventory       │ ──────► 7. If below threshold:
│ Service Logic   │            Publish InventoryLowEvent
└──────┬──────────┘
       │
       ▼
┌─────────────────┐
│  EventBridge    │
└──────┬──────────┘
       │ 8. Route to alert service
       ▼
┌─────────────────┐
│  Alert Service  │ ──────► 9. Create alert in RDS
│   (Lambda)      │        10. Determine suppliers
└──────┬──────────┘
       │
       ▼
┌─────────────────┐
│      SNS        │ ──────► 11. Send SMS/Email to suppliers
└─────────────────┘        12. Push notification to facility manager
                           13. Update Grafana dashboard
```

### 3.3 Disaster Detection → Emergency Response

```
┌─────────────────┐
│ Analytics       │ ──────► 1. Continuous monitoring
│ Service         │         2. Aggregate symptom reports
│ (Lambda/Cron)   │            (5-minute windows)
└──────┬──────────┘
       │ 3. Calculate surge metrics
       ▼
┌─────────────────┐
│ Surge Detection │ ──────► 4. Statistical analysis
│   Algorithm     │         5. Geospatial clustering
└──────┬──────────┘
       │ 6. If surge detected (severity ≥ 4)
       ▼
┌─────────────────┐
│  EventBridge    │ ──────► 7. Publish SurgeDetectedEvent
└──────┬──────────┘
       │
       ▼
┌─────────────────┐
│  Step Functions │ ──────► 8. Start DisasterResponseWorkflow
│ State Machine   │
└──────┬──────────┘
       │ 9. Parallel execution:
       ├──────────────────────────────────┐
       │                                  │
       ▼                                  ▼
┌─────────────────┐              ┌─────────────────┐
│ Notify Govt     │              │ Alert Hospitals │
│ Officials (SNS) │              │   (SNS + SMS)   │
└─────────────────┘              └─────────────────┘
       │                                  │
       ▼                                  ▼
┌─────────────────┐              ┌─────────────────┐
│Request Emergency│              │Deploy Mobile    │
│Supplies (Lambda)│              │Units (Lambda)   │
└─────────────────┘              └─────────────────┘
       │                                  │
       └──────────┬───────────────────────┘
                  │ 10. All tasks complete
                  ▼
         ┌─────────────────┐
         │Monitor Situation│ ──────► 11. Wait 1 hour
         │  (Wait State)   │         12. Check resolution
         └─────────────────┘         13. Loop until resolved
```

---

## 4. Security Design

### 4.1 AWS IAM (Identity and Access Management)

**Role-Based Access Control**

```
Roles:
1. PatientRole
   - Permissions: Read own records, submit symptoms, view treatment plans
   - Resources: /patients/{patient_id}/*

2. DoctorRole
   - Permissions: Read patient records, approve diagnoses, create prescriptions
   - Resources: /patients/*, /diagnoses/*, /prescriptions/*

3. SupplierRole
   - Permissions: Read inventory alerts, update delivery status
   - Resources: /inventory/alerts, /orders/*

4. GovernmentOfficialRole
   - Permissions: Read all analytics, view outbreak data
   - Resources: /analytics/*, /alerts/*

5. LambdaExecutionRole
   - Permissions: CloudWatch Logs, RDS access, SageMaker invoke, SNS publish
   - Principle of least privilege per function
```

**Service Control Policies**
- Deny deletion of critical resources (RDS, S3 buckets)
- Enforce encryption for all data storage
- Restrict access to production environment
- MFA required for privileged operations

### 4.2 Data Encryption

**Encryption at Rest**
- RDS: AES-256 encryption using AWS KMS
- S3: Server-side encryption (SSE-KMS)
- DynamoDB: Encryption enabled by default
- EBS volumes: Encrypted with KMS keys
- Timestream: Automatic encryption

**Encryption in Transit**
- TLS 1.3 for all API communications
- Certificate management via AWS Certificate Manager
- VPC endpoints for private AWS service access
- No unencrypted HTTP traffic allowed

**Key Management**
- AWS KMS for encryption key management
- Automatic key rotation (annual)
- Separate keys per environment (dev, staging, prod)
- Customer-managed keys (CMK) for sensitive data
- Key usage auditing via CloudTrail

### 4.3 API Authentication & Authorization

**JWT Token-Based Authentication**

```
Flow:
1. User login → Authentication Service
2. Validate credentials against user database
3. Generate JWT token with claims:
   {
     "sub": "user_id",
     "role": "doctor",
     "facility_id": "fac-123",
     "exp": 1708099200,
     "iat": 1708095600
   }
4. Return token to client
5. Client includes token in Authorization header
6. API Gateway validates token using Lambda authorizer
7. Extract claims and pass to backend services
```

**Lambda Authorizer**
- Validates JWT signature
- Checks token expiration
- Verifies user role and permissions
- Caches authorization decisions (5 minutes)
- Returns IAM policy for API Gateway

**Multi-Factor Authentication (MFA)**
- Required for doctors and government officials
- SMS-based OTP via Twilio
- Time-based OTP (TOTP) support
- Backup codes for account recovery

### 4.4 HIPAA Compliance

**Technical Safeguards**
- Unique user identification (UUID-based)
- Automatic logoff after 15 minutes of inactivity
- Encryption of ePHI (Electronic Protected Health Information)
- Audit controls and access logs
- Integrity controls (checksums, digital signatures)

**Administrative Safeguards**
- Security management process
- Workforce security training
- Information access management
- Security incident procedures
- Business associate agreements (BAAs) with AWS

**Physical Safeguards**
- AWS data centers with SOC 2 compliance
- Facility access controls
- Workstation security policies
- Device and media controls

**Audit Logging**
- CloudTrail for all AWS API calls
- Application-level audit logs in RDS
- Log retention: 7 years (HIPAA requirement)
- Tamper-proof logs (write-once S3 buckets)
- Regular log reviews and anomaly detection

**Data Anonymization**
- PII removed from ML training datasets
- Differential privacy techniques for analytics
- Data masking in non-production environments
- Secure data disposal procedures

### 4.5 Network Security

**VPC Architecture**
- Multi-AZ deployment across 3 availability zones
- Public subnets: API Gateway, Load Balancers
- Private subnets: Lambda, RDS, SageMaker
- Isolated subnets: Database replicas

**Security Groups**
- Principle of least privilege
- Inbound rules restricted to necessary ports
- Database access only from application layer
- No direct internet access to databases

**Network ACLs**
- Stateless firewall rules
- Deny all by default
- Allow specific CIDR ranges

**AWS WAF (Web Application Firewall)**
- SQL injection protection
- XSS (Cross-Site Scripting) prevention
- Rate limiting (1000 requests/5 minutes per IP)
- Geo-blocking for non-target regions
- Bot detection and mitigation

---

## 5. Scalability & Performance

### 5.1 Auto-Scaling Strategies

**Lambda Auto-Scaling**
- Concurrent execution limit: 1000 (default)
- Reserved concurrency for critical functions:
  - Diagnosis Service: 200
  - Alert Service: 100
  - Patient Service: 150
- Provisioned concurrency for low-latency: 50 instances
- Burst capacity: Up to 3000 concurrent executions

**RDS Auto-Scaling**
- Read replica auto-scaling: 2-5 replicas based on CPU
- Storage auto-scaling: 500GB → 2TB (10% threshold)
- Vertical scaling: Automated instance size increase during peak hours
- Connection pooling: RDS Proxy with 1000 max connections

**SageMaker Endpoint Auto-Scaling**
- Target tracking: 70% invocations per instance
- Min instances: 2 (high availability)
- Max instances: 10
- Scale-up cooldown: 60 seconds
- Scale-down cooldown: 300 seconds

**API Gateway Throttling**
- Account-level: 10,000 requests/second
- Per-method: 1,000 requests/second
- Burst capacity: 5,000 requests
- Usage plans for different user tiers

### 5.2 Caching Mechanisms

**API Gateway Caching**
- Cache TTL: 300 seconds (5 minutes)
- Cache capacity: 6.1 GB
- Cached endpoints:
  - GET /medications (inventory list)
  - GET /facilities (facility information)
  - GET /analytics/summary (dashboard data)
- Cache invalidation on data updates

**Application-Level Caching (Redis/ElastiCache)**
- Session data: 30-minute TTL
- User profiles: 1-hour TTL
- Medication catalog: 24-hour TTL
- Treatment guidelines: 7-day TTL
- Cluster mode: 3 nodes with automatic failover

**CloudFront CDN**
- Static assets (images, CSS, JS): 24-hour cache
- Edge locations: Global distribution
- Origin: S3 bucket for frontend assets
- Compression: Gzip/Brotli enabled
- Cache invalidation on deployment

**Database Query Caching**
- RDS query cache: Enabled for read replicas
- Application-level caching: Frequently accessed patient records
- Cache-aside pattern: Check cache → Query DB → Update cache

### 5.3 Database Optimization

**Indexing Strategy**

```sql
-- Composite indexes for common queries
CREATE INDEX idx_symptom_reports_patient_date 
ON symptom_reports(patient_id, reported_at DESC);

CREATE INDEX idx_inventory_facility_medication 
ON inventory(facility_id, medication_name);

CREATE INDEX idx_diagnoses_report_doctor 
ON diagnoses(report_id, doctor_id);

-- Partial indexes for filtered queries
CREATE INDEX idx_active_treatment_plans 
ON treatment_plans(patient_id) 
WHERE status = 'active';

-- GiST index for geospatial queries
CREATE INDEX idx_symptom_reports_location 
ON symptom_reports USING GIST(location);
```

**Query Optimization**
- Use EXPLAIN ANALYZE for slow queries
- Avoid N+1 queries (use JOINs or batch loading)
- Pagination for large result sets (LIMIT/OFFSET)
- Materialized views for complex analytics
- Partitioning for large tables (by date/region)

**Connection Pooling**
- RDS Proxy: Multiplexes connections
- Lambda: Reuse database connections across invocations
- Connection timeout: 30 seconds
- Max connections per Lambda: 10

**Read/Write Splitting**
- Write operations: Primary RDS instance
- Read operations: Read replicas (round-robin)
- Eventual consistency acceptable for analytics
- Strong consistency for critical operations

### 5.4 Performance Targets

**API Latency**
- P50: < 200ms
- P95: < 500ms
- P99: < 1000ms

**AI Model Inference**
- Symptom analysis: < 2 seconds
- Treatment recommendation: < 1 second
- Inventory prediction: Batch processing (acceptable delay)

**Database Queries**
- Simple queries: < 50ms
- Complex analytics: < 2 seconds
- Geospatial queries: < 500ms

**Availability**
- Uptime SLA: 99.9% (8.76 hours downtime/year)
- Multi-AZ deployment for high availability
- Automated failover: < 2 minutes

---

## 6. Disaster Mode Design

### 6.1 Surge Detection Algorithm

**Multi-Factor Detection**

```python
class SurgeDetector:
    def __init__(self):
        self.baseline_window = 30  # days
        self.detection_window = 60  # minutes
        self.threshold_z_score = 2.5
        self.geo_cluster_threshold = 5  # km radius
        
    def detect(self, current_data, historical_data):
        # Factor 1: Statistical anomaly
        statistical_surge = self._statistical_detection(
            current_data, historical_data
        )
        
        # Factor 2: Geospatial clustering
        geo_clusters = self._geospatial_clustering(current_data)
        
        # Factor 3: Symptom pattern matching
        symptom_match = self._symptom_pattern_analysis(current_data)
        
        # Combine factors
        severity = self._calculate_severity(
            statistical_surge, geo_clusters, symptom_match
        )
        
        return {
            "surge_detected": severity >= 2,
            "severity": severity,
            "factors": {
                "statistical": statistical_surge,
                "geospatial": geo_clusters,
                "symptom_pattern": symptom_match
            }
        }
```

**Severity Levels**

```
Level 1 (Normal): Z-score < 2.5, no action
Level 2 (Mild): Z-score 2.5-3.0, send warnings to local health workers
Level 3 (Moderate): Z-score 3.0-4.0, alert district hospitals, increase monitoring
Level 4 (Severe): Z-score 4.0-5.0, activate disaster mode, notify government
Level 5 (Critical): Z-score > 5.0, full emergency response, deploy mobile units
```

### 6.2 Emergency Workflow Triggers

**Automatic Triggers**
- Surge detection severity ≥ 4
- Inventory depletion of critical medications (< 10% stock)
- Multiple facility capacity exceeded (> 90% bed occupancy)
- Rapid symptom spread (> 50% increase in 24 hours)

**Manual Triggers**
- Government official override
- Doctor emergency declaration
- External disaster notification (flood, earthquake)

**Trigger Actions**
1. Publish `EmergencyActivatedEvent` to EventBridge
2. Start Step Functions state machine
3. Update system status to "DISASTER_MODE"
4. Enable emergency protocols in all services
5. Increase Lambda concurrency limits
6. Activate backup communication channels

### 6.3 Hospital Coordination Protocol

**Capacity Management**

```
Hospital Coordination Workflow:
1. Query all hospitals in affected region
2. Retrieve current capacity:
   - Total beds
   - Occupied beds
   - Available beds
   - ICU capacity
   - Ventilator availability
3. Calculate regional capacity utilization
4. Identify hospitals with available capacity
5. Route patients to nearest available facility
6. Update routing recommendations in real-time
```

**Inter-Hospital Communication**
- Dedicated SNS topic: `HospitalCoordination`
- Real-time bed availability updates
- Patient transfer requests
- Resource sharing (medications, equipment)
- Staff reallocation coordination

**Patient Routing Algorithm**
```python
def route_patient(patient_location, severity, hospitals):
    available_hospitals = [
        h for h in hospitals 
        if h.available_beds > 0 and h.can_handle_severity(severity)
    ]
    
    # Sort by distance and capacity
    scored_hospitals = [
        {
            "hospital": h,
            "score": calculate_score(
                distance=haversine(patient_location, h.location),
                available_beds=h.available_beds,
                specialization_match=h.has_specialization(severity)
            )
        }
        for h in available_hospitals
    ]
    
    return sorted(scored_hospitals, key=lambda x: x["score"], reverse=True)[0]
```

### 6.4 Government Notification System

**Notification Hierarchy**

```
Level 1: Local Health Officer
- SMS + Email + Push notification
- Response required within 30 minutes

Level 2: District Health Department
- SMS + Email + Phone call (automated)
- Response required within 1 hour

Level 3: State Health Ministry
- SMS + Email + Phone call + Dashboard alert
- Response required within 2 hours

Level 4: National Health Authority
- All channels + Emergency hotline activation
- Immediate response required
```

**Notification Content**

```json
{
  "alert_id": "alert-789",
  "type": "DISEASE_OUTBREAK",
  "severity": 4,
  "region": {
    "district": "Rural District A",
    "state": "State X",
    "affected_villages": ["Village 1", "Village 2", "Village 3"]
  },
  "outbreak_details": {
    "suspected_disease": "Dengue Fever",
    "confirmed_cases": 45,
    "suspected_cases": 120,
    "deaths": 2,
    "onset_date": "2026-02-10"
  },
  "current_response": {
    "hospitals_alerted": 5,
    "mobile_units_deployed": 2,
    "emergency_supplies_requested": true
  },
  "required_actions": [
    "Approve emergency funding",
    "Deploy additional medical staff",
    "Coordinate with neighboring districts",
    "Initiate public awareness campaign"
  ],
  "dashboard_link": "https://pulsenet.gov/emergency/alert-789",
  "contact": {
    "incident_commander": "Dr. [name]",
    "phone": "[phone_number]",
    "email": "[email]"
  }
}
```

**Escalation Rules**
- No response in 30 minutes → Escalate to next level
- Severity increases → Immediate escalation
- Multi-district outbreak → Skip to state level
- Cross-border outbreak → National level notification

### 6.5 Emergency Resource Allocation

**Automated Actions**
1. **Medication Redistribution**
   - Query inventory across all facilities
   - Identify surplus stock in unaffected areas
   - Generate transfer orders
   - Notify logistics coordinators

2. **Staff Reallocation**
   - Identify available medical staff
   - Match skills to outbreak needs
   - Generate deployment schedules
   - Arrange transportation

3. **Mobile Unit Deployment**
   - Activate standby mobile clinics
   - Route to affected areas
   - Pre-stock with predicted medications
   - Assign medical teams

4. **Supply Chain Acceleration**
   - Trigger emergency orders with suppliers
   - Expedited shipping arrangements
   - Customs clearance automation
   - Real-time delivery tracking

---

## 7. Key Design Decisions

### 7.1 Why AWS SageMaker Over Custom ML Infrastructure?

**Rationale**
- **Managed Service**: No infrastructure management overhead
- **Built-in MLOps**: Model registry, versioning, A/B testing
- **Auto-Scaling**: Handles variable inference loads automatically
- **Cost-Effective**: Pay-per-use, no idle resource costs
- **Integration**: Seamless with Lambda, S3, CloudWatch
- **Security**: Built-in encryption, VPC support, IAM integration
- **Monitoring**: Native CloudWatch metrics and alarms

**Trade-offs**
- Vendor lock-in (mitigated by using standard frameworks like TensorFlow)
- Less control over infrastructure (acceptable for managed service benefits)
- Slightly higher cost than self-managed (offset by reduced operational overhead)

### 7.2 Serverless vs. Containerized Architecture

**Decision: Hybrid Approach**

**Serverless (Lambda) for:**
- API endpoints (stateless, variable load)
- Event processing (EventBridge triggers)
- Scheduled tasks (CloudWatch Events)
- Short-lived operations (< 15 minutes)

**Rationale**
- Zero server management
- Automatic scaling (0 to 1000s of instances)
- Pay-per-invocation (cost-effective for variable workloads)
- Built-in high availability
- Fast deployment and iteration

**Containerized (ECS/Fargate) for:**
- Long-running ML training jobs (if needed beyond SageMaker)
- Complex data processing pipelines
- Legacy system integrations
- Services requiring persistent connections

**Why Not Fully Containerized?**
- Higher operational overhead (cluster management)
- Less cost-effective for variable workloads
- Slower cold start for infrequent operations
- More complex auto-scaling configuration

### 7.3 Choice of Timestream for Inventory Tracking

**Rationale**
- **Purpose-Built**: Optimized for time-series data (inventory over time)
- **Cost**: 1/10th the cost of relational databases for time-series
- **Performance**: Fast queries for trend analysis and forecasting
- **Automatic Tiering**: Hot/warm/cold storage based on age
- **Built-in Functions**: Interpolation, aggregation, windowing
- **Scalability**: Handles trillions of events per day
- **Integration**: Native support for Grafana dashboards

**Alternatives Considered**
- **RDS**: Too expensive for high-frequency time-series data
- **DynamoDB**: Requires complex query patterns for time-series
- **InfluxDB**: Additional operational overhead, not managed
- **CloudWatch Metrics**: Limited retention and query capabilities

**Use Case Fit**
- Track medication consumption every time a prescription is dispensed
- Analyze consumption trends over time
- Predict future demand based on historical patterns
- Generate real-time dashboards in Grafana
- Cost-effective storage for years of historical data

### 7.4 Event-Driven Architecture Benefits

**Why Event-Driven?**

1. **Decoupling**
   - Services don't need to know about each other
   - Easy to add new consumers without modifying producers
   - Independent deployment and scaling

2. **Scalability**
   - Asynchronous processing handles traffic spikes
   - Queue-based buffering prevents system overload
   - Parallel processing of events

3. **Resilience**
   - Failed events can be retried automatically
   - Dead-letter queues for problematic events
   - Circuit breaker patterns prevent cascading failures

4. **Flexibility**
   - Easy to add new workflows (new EventBridge rules)
   - A/B testing with event routing
   - Audit trail of all system events

5. **Real-Time Processing**
   - Immediate reaction to critical events (inventory low, outbreak detected)
   - No polling overhead
   - Push-based notifications

**Example: Inventory Depletion**
```
Traditional Approach:
- Pharmacy service updates inventory
- Pharmacy service checks reorder point
- Pharmacy service calls supplier service
- Pharmacy service calls alert service
- Tight coupling, synchronous calls, single point of failure

Event-Driven Approach:
- Pharmacy service updates inventory
- Pharmacy service publishes InventoryUpdatedEvent
- Multiple consumers react independently:
  * Inventory service checks reorder point
  * Alert service creates notifications
  * Analytics service updates dashboards
  * Supplier service receives orders
- Loose coupling, asynchronous, resilient
```

### 7.5 PostgreSQL vs. DynamoDB

**Decision: PostgreSQL (RDS) as Primary Database**

**Rationale**
- **Relational Data**: Patient records, prescriptions, treatment plans have complex relationships
- **ACID Compliance**: Critical for healthcare data integrity
- **Complex Queries**: JOINs, aggregations, geospatial queries (PostGIS)
- **Mature Ecosystem**: Well-understood, extensive tooling
- **HIPAA Compliance**: Easier audit trails and access controls
- **Data Consistency**: Strong consistency for medical records

**When to Use DynamoDB**
- Session management (high read/write, simple key-value)
- Device tokens for push notifications
- Real-time leaderboards or counters
- Extremely high-scale scenarios (millions of requests/second)

**Hybrid Approach**
- RDS for transactional data (patients, diagnoses, prescriptions)
- DynamoDB for operational data (sessions, device tokens)
- Timestream for time-series data (inventory tracking)
- S3 for unstructured data (medical images, documents)

### 7.6 Multi-Region vs. Single-Region Deployment

**Initial Decision: Single-Region with Multi-AZ**

**Rationale**
- **Cost**: Multi-region significantly more expensive
- **Complexity**: Data replication, consistency challenges
- **Latency**: Rural areas typically served by single region
- **Compliance**: Data residency requirements may restrict regions

**Multi-AZ Benefits**
- High availability within region (99.99% uptime)
- Automatic failover (< 2 minutes)
- Data replication across availability zones
- Protection against data center failures

**Future Multi-Region Considerations**
- When expanding to multiple countries
- For disaster recovery (DR) in separate region
- To reduce latency for geographically distributed users
- Active-passive setup: Primary region + DR region

### 7.7 Synchronous vs. Asynchronous Processing

**Decision Matrix**

**Synchronous (API Gateway → Lambda → Response)**
- User authentication
- Patient profile retrieval
- Prescription lookup
- Real-time symptom submission (with async AI processing)

**Asynchronous (EventBridge → SQS → Lambda)**
- AI model inference (symptom analysis)
- Inventory predictions
- Analytics aggregation
- Notification delivery
- Report generation

**Rationale**
- User-facing operations need immediate feedback
- Heavy computations should not block API responses
- Background tasks can tolerate delays
- Async processing enables retry logic and error handling

---

## 8. Technology Stack Summary

### Frontend
- **Mobile**: Flutter (iOS/Android)
- **Web**: React.js + TypeScript
- **State Management**: Redux Toolkit
- **Offline Storage**: SQLite, Hive
- **UI Components**: Material Design

### Backend
- **API Gateway**: AWS API Gateway (REST + WebSocket)
- **Compute**: AWS Lambda (Python 3.11, Node.js 20.x)
- **Frameworks**: FastAPI, Express.js
- **Authentication**: JWT, AWS Cognito (optional)

### AI/ML
- **Training**: AWS SageMaker, TensorFlow 2.x, PyTorch
- **Models**: BERT, XGBoost, LSTM
- **Inference**: SageMaker Endpoints
- **Feature Store**: SageMaker Feature Store

### Data
- **Relational**: Amazon RDS (PostgreSQL 15)
- **Time-Series**: Amazon Timestream
- **NoSQL**: Amazon DynamoDB (optional)
- **Object Storage**: Amazon S3
- **Caching**: Amazon ElastiCache (Redis)

### Integration
- **Events**: Amazon EventBridge
- **Messaging**: Amazon SNS, SQS
- **Streaming**: Apache Kafka (Amazon MSK)
- **Orchestration**: AWS Step Functions

### Monitoring
- **Metrics**: Amazon CloudWatch
- **Logs**: CloudWatch Logs
- **Dashboards**: Grafana, CloudWatch Dashboards
- **Tracing**: AWS X-Ray
- **Alerting**: CloudWatch Alarms, SNS

### Security
- **IAM**: AWS IAM
- **Encryption**: AWS KMS
- **Secrets**: AWS Secrets Manager
- **WAF**: AWS WAF
- **Network**: VPC, Security Groups, NACLs

### DevOps
- **IaC**: AWS CloudFormation, Terraform
- **CI/CD**: AWS CodePipeline, GitHub Actions
- **Version Control**: Git, GitHub
- **Containerization**: Docker (if needed)

---

## 9. Deployment Architecture

### Environment Strategy

**Development**
- Single AZ deployment
- Smaller instance sizes
- Reduced Lambda concurrency
- Sample data for testing

**Staging**
- Multi-AZ deployment
- Production-like configuration
- Anonymized production data
- Full integration testing

**Production**
- Multi-AZ across 3 availability zones
- Auto-scaling enabled
- Full monitoring and alerting
- Automated backups and disaster recovery

### Infrastructure as Code

**Terraform Modules**
```
modules/
├── networking/          # VPC, subnets, security groups
├── compute/             # Lambda functions, layers
├── database/            # RDS, Timestream, DynamoDB
├── ai-ml/               # SageMaker endpoints, models
├── integration/         # EventBridge, SNS, SQS
├── monitoring/          # CloudWatch, Grafana
└── security/            # IAM roles, KMS keys, WAF
```

### CI/CD Pipeline

```
1. Code Commit (GitHub)
   ↓
2. Automated Tests (Unit, Integration)
   ↓
3. Build (Lambda packages, Docker images)
   ↓
4. Deploy to Staging
   ↓
5. Automated E2E Tests
   ↓
6. Manual Approval
   ↓
7. Deploy to Production (Blue-Green)
   ↓
8. Health Checks
   ↓
9. Rollback if Issues Detected
```

---

## 10. Cost Optimization Strategies

1. **Serverless First**: Pay only for actual usage
2. **Reserved Capacity**: RDS reserved instances (40% savings)
3. **Spot Instances**: SageMaker training jobs (70% savings)
4. **S3 Lifecycle Policies**: Archive old data to Glacier
5. **CloudWatch Log Retention**: 30 days for most logs
6. **Lambda Memory Optimization**: Right-size memory allocation
7. **API Gateway Caching**: Reduce backend calls
8. **DynamoDB On-Demand**: For variable workloads
9. **Data Transfer Optimization**: Use VPC endpoints
10. **Monitoring**: Cost anomaly detection with AWS Cost Explorer

**Estimated Monthly Cost (1000 patients/day)**
- Lambda: $200
- RDS: $400 (db.r6g.xlarge)
- SageMaker: $300 (inference endpoints)
- S3: $50
- Timestream: $100
- EventBridge/SNS/SQS: $50
- CloudWatch: $100
- Data Transfer: $100
- **Total: ~$1,300/month**

---

## 11. Future Enhancements

1. **Telemedicine Integration**: Video consultation via WebRTC
2. **Wearable Device Integration**: IoT data from health monitors
3. **Blockchain for Records**: Immutable medical record storage
4. **Advanced AI**: Computer vision for medical image analysis
5. **Voice Interface**: Symptom reporting via voice (Alexa/Google)
6. **Predictive Analytics**: Disease outbreak prediction models
7. **Mobile Offline Mode**: Full offline functionality with sync
8. **Multi-Language Support**: Regional language interfaces
9. **Integration with National Health Systems**: Data sharing protocols
10. **Drone Delivery**: Medication delivery via drones in remote areas

---

## Document Version

- **Version**: 1.0
- **Last Updated**: February 15, 2026
- **Authors**: PulseNet Architecture Team
- **Status**: Approved for Implementation

---

## References

- AWS Well-Architected Framework
- HIPAA Security Rule
- HL7 FHIR Standards
- WHO Digital Health Guidelines
- AWS SageMaker Best Practices
- Microservices Design Patterns
