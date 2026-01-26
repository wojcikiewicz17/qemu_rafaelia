/*
 * RAFAELIA MVP Module System - Implementation
 * Professional Add-on Architecture with 42 Interconnected Modules
 * 
 * TECHNICAL SPECIFICATION: Version 1.0.0
 * Classification: Professional/Enterprise
 * 
 * FIAT_PORTAL :: 龍空神 { ARKREΩ_CORE + STACK128K_HYPER + ALG_RAFAELIA_RING }
 */

#include "hw/core/rafaelia-mvp-modules.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"
#include <math.h>
#include <stdio.h>

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 1: MODULE INFORMATION TABLE
 * Complete technical documentation for all 42 MVP modules
 * ═══════════════════════════════════════════════════════════════════════════
 */

static const rafaelia_module_info_t g_module_info[RAFAELIA_MVP_MODULE_COUNT] = {
    /* Category A: Core Foundation Modules (01-06) */
    {
        .id = MVP_MODULE_KERNEL_CORE,
        .name = "Kernel Core",
        .short_description = "Core kernel operations and system primitives",
        .technical_description = "Implements fundamental kernel operations including "
            "interrupt handling, context switching, and low-level system calls. "
            "Based on microkernel architecture principles (Liedtke, 1995).",
        .category = CATEGORY_CORE_FOUNDATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_SYNC,
        .scientific_foundation = "Microkernel Architecture Theory"
    },
    {
        .id = MVP_MODULE_MEMORY_MANAGER,
        .name = "Memory Manager",
        .short_description = "Memory allocation and management subsystem",
        .technical_description = "Provides memory allocation, deallocation, and tracking "
            "using buddy system and slab allocation algorithms. Implements memory "
            "protection and virtual memory mapping.",
        .category = CATEGORY_CORE_FOUNDATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_SYNC,
        .scientific_foundation = "Buddy System Algorithm (Knowlton, 1965)"
    },
    {
        .id = MVP_MODULE_PROCESS_SCHEDULER,
        .name = "Process Scheduler",
        .short_description = "Process scheduling and execution management",
        .technical_description = "Implements multi-level feedback queue scheduling with "
            "priority-based preemption. Supports real-time scheduling policies "
            "including Rate Monotonic and Earliest Deadline First.",
        .category = CATEGORY_CORE_FOUNDATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_REAL_TIME,
        .scientific_foundation = "MLFQ Scheduling (Corbató, 1962)"
    },
    {
        .id = MVP_MODULE_EVENT_DISPATCHER,
        .name = "Event Dispatcher",
        .short_description = "Event handling and dispatch system",
        .technical_description = "Provides asynchronous event processing using reactor "
            "pattern. Implements priority queuing and event coalescing for "
            "optimal throughput.",
        .category = CATEGORY_CORE_FOUNDATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_ASYNC | MODULE_CAP_STREAMING,
        .scientific_foundation = "Reactor Pattern (Schmidt, 1995)"
    },
    {
        .id = MVP_MODULE_RESOURCE_ALLOCATOR,
        .name = "Resource Allocator",
        .short_description = "System resource allocation and tracking",
        .technical_description = "Manages allocation of system resources including CPU time, "
            "memory, I/O bandwidth, and network capacity. Implements fair-share "
            "scheduling and resource quotas.",
        .category = CATEGORY_CORE_FOUNDATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_MONITORING,
        .scientific_foundation = "Fair-Share Scheduling (Kay & Lauder, 1988)"
    },
    {
        .id = MVP_MODULE_CONFIGURATION_MANAGER,
        .name = "Configuration Manager",
        .short_description = "System configuration and settings management",
        .technical_description = "Provides hierarchical configuration management with "
            "validation, versioning, and hot-reload capabilities. Supports "
            "environment-specific overrides.",
        .category = CATEGORY_CORE_FOUNDATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_PERSISTENCE,
        .scientific_foundation = "Configuration Management Theory"
    },

    /* Category B: Cognitive Processing Modules (07-12) */
    {
        .id = MVP_MODULE_CYCLE_PROCESSOR,
        .name = "Cycle Processor",
        .short_description = "ψχρΔΣΩ cognitive cycle operations",
        .technical_description = "Implements the RAFAELIA ψχρΔΣΩ cognitive cycle: "
            "ψ(Intention) → χ(Observation) → ρ(Noise) → Δ(Transmutation) → "
            "Σ(Memory) → Ω(Completion). Based on cognitive architecture theory.",
        .category = CATEGORY_COGNITIVE,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_ETHICAL,
        .scientific_foundation = "Cognitive Architecture (Newell & Simon, 1972)"
    },
    {
        .id = MVP_MODULE_ETHICAL_FILTER,
        .name = "Ethical Filter",
        .short_description = "Φ_ethica ethical computation engine",
        .technical_description = "Computes ethical metrics using formula: "
            "Φ_ethica = Min(Entropia) × Max(Coerência). Implements extended "
            "formula: Φ_ethica^∞ = e^{(Amor+Verbo)·(Verdade/Consciência)} - 1",
        .category = CATEGORY_COGNITIVE,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATELESS | MODULE_CAP_ETHICAL,
        .scientific_foundation = "Ethical Computing Framework"
    },
    {
        .id = MVP_MODULE_WISDOM_CALCULATOR,
        .name = "Wisdom Calculator",
        .short_description = "OWLψ operational wisdom metrics",
        .technical_description = "Calculates Operational Wisdom Level using formula: "
            "OWLψ = φ^(insight × ethics) × √(flow). Provides wisdom-based "
            "decision support and quality assessment.",
        .category = CATEGORY_COGNITIVE,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATELESS | MODULE_CAP_ETHICAL,
        .scientific_foundation = "Wisdom Research (Sternberg, 1990)"
    },
    {
        .id = MVP_MODULE_PATTERN_RECOGNIZER,
        .name = "Pattern Recognizer",
        .short_description = "Pattern recognition and matching engine",
        .technical_description = "Implements pattern recognition using statistical "
            "and neural network approaches. Supports template matching, "
            "feature extraction, and classification.",
        .category = CATEGORY_COGNITIVE,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_LEARNING | MODULE_CAP_BATCH,
        .scientific_foundation = "Pattern Recognition Theory (Duda & Hart, 1973)"
    },
    {
        .id = MVP_MODULE_DECISION_ENGINE,
        .name = "Decision Engine",
        .short_description = "Decision making and reasoning logic",
        .technical_description = "Provides decision-making capabilities using "
            "multi-criteria decision analysis, fuzzy logic, and Bayesian "
            "reasoning. Integrates ethical constraints.",
        .category = CATEGORY_COGNITIVE,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_ETHICAL | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Decision Theory (von Neumann & Morgenstern, 1944)"
    },
    {
        .id = MVP_MODULE_LEARNING_SUBSYSTEM,
        .name = "Learning Subsystem",
        .short_description = "Machine learning and adaptation",
        .technical_description = "Implements supervised, unsupervised, and reinforcement "
            "learning algorithms. Supports online learning and model adaptation "
            "with ethical constraints.",
        .category = CATEGORY_COGNITIVE,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_LEARNING | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Machine Learning Theory (Mitchell, 1997)"
    },

    /* Category C: Data Management Modules (13-18) */
    {
        .id = MVP_MODULE_BLOCK_STORAGE,
        .name = "Block Storage",
        .short_description = "Knowledge block management system",
        .technical_description = "Manages RAFAELIA knowledge blocks (Bloco_n) with "
            "structure: {id, posição, coeficientes[33], atitudes[33], estado, "
            "observações, ações_futuras, retroalimentação}.",
        .category = CATEGORY_DATA_MANAGEMENT,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_PERSISTENCE | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Knowledge Representation Theory"
    },
    {
        .id = MVP_MODULE_CACHE_CONTROLLER,
        .name = "Cache Controller",
        .short_description = "Multi-level caching strategies",
        .technical_description = "Implements LRU, LFU, and ARC caching algorithms "
            "with configurable eviction policies. Supports write-through "
            "and write-back modes.",
        .category = CATEGORY_DATA_MANAGEMENT,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_CACHEABLE | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Cache Replacement Algorithms (Megiddo & Modha, 2003)"
    },
    {
        .id = MVP_MODULE_PERSISTENCE_LAYER,
        .name = "Persistence Layer",
        .short_description = "Data persistence and durability",
        .technical_description = "Provides ACID-compliant data persistence with "
            "write-ahead logging, checkpointing, and crash recovery. "
            "Supports multiple storage backends.",
        .category = CATEGORY_DATA_MANAGEMENT,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_PERSISTENCE | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Database Recovery Theory (Mohan et al., 1992)"
    },
    {
        .id = MVP_MODULE_SERIALIZATION_ENGINE,
        .name = "Serialization Engine",
        .short_description = "Data serialization and deserialization",
        .technical_description = "Implements efficient binary and text serialization "
            "formats. Supports schema evolution and backward compatibility. "
            "Includes Protocol Buffers and MessagePack support.",
        .category = CATEGORY_DATA_MANAGEMENT,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATELESS | MODULE_CAP_STREAMING,
        .scientific_foundation = "Data Serialization Theory"
    },
    {
        .id = MVP_MODULE_COMPRESSION_CODEC,
        .name = "Compression Codec",
        .short_description = "Data compression algorithms",
        .technical_description = "Provides lossless compression using LZ77, DEFLATE, "
            "and Zstandard algorithms. Supports streaming compression "
            "and adaptive algorithm selection.",
        .category = CATEGORY_DATA_MANAGEMENT,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATELESS | MODULE_CAP_STREAMING,
        .scientific_foundation = "Information Theory (Shannon, 1948)"
    },
    {
        .id = MVP_MODULE_INDEX_MANAGER,
        .name = "Index Manager",
        .short_description = "Indexing and search optimization",
        .technical_description = "Implements B-tree, hash, and inverted index structures. "
            "Provides full-text search, range queries, and prefix matching. "
            "Supports index maintenance and optimization.",
        .category = CATEGORY_DATA_MANAGEMENT,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_PERSISTENCE,
        .scientific_foundation = "Index Data Structures (Bayer & McCreight, 1972)"
    },

    /* Category D: Communication Modules (19-24) */
    {
        .id = MVP_MODULE_MESSAGE_BROKER,
        .name = "Message Broker",
        .short_description = "Inter-module messaging system",
        .technical_description = "Provides reliable message queuing with guaranteed "
            "delivery, priority queuing, and dead-letter handling. "
            "Implements both point-to-point and publish-subscribe patterns.",
        .category = CATEGORY_COMMUNICATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_ASYNC | MODULE_CAP_DISTRIBUTED,
        .scientific_foundation = "Message-Oriented Middleware Theory"
    },
    {
        .id = MVP_MODULE_PROTOCOL_HANDLER,
        .name = "Protocol Handler",
        .short_description = "Communication protocol implementation",
        .technical_description = "Implements multiple communication protocols including "
            "TCP/IP, UDP, and custom RAFAELIA protocols. Provides protocol "
            "negotiation and version management.",
        .category = CATEGORY_COMMUNICATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_STREAMING,
        .scientific_foundation = "Protocol Engineering (Holzmann, 1991)"
    },
    {
        .id = MVP_MODULE_STREAM_PROCESSOR,
        .name = "Stream Processor",
        .short_description = "Stream data processing pipeline",
        .technical_description = "Implements stream processing with windowing, "
            "aggregation, and join operations. Supports exactly-once "
            "semantics and backpressure handling.",
        .category = CATEGORY_COMMUNICATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STREAMING | MODULE_CAP_ASYNC,
        .scientific_foundation = "Stream Processing Theory (Abadi et al., 2003)"
    },
    {
        .id = MVP_MODULE_RPC_GATEWAY,
        .name = "RPC Gateway",
        .short_description = "Remote procedure call infrastructure",
        .technical_description = "Provides RPC capabilities with automatic serialization, "
            "load balancing, and circuit breaker patterns. Supports "
            "synchronous and asynchronous invocation.",
        .category = CATEGORY_COMMUNICATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_DISTRIBUTED | MODULE_CAP_SYNC,
        .scientific_foundation = "RPC Theory (Birrell & Nelson, 1984)"
    },
    {
        .id = MVP_MODULE_EVENT_BUS,
        .name = "Event Bus",
        .short_description = "Publish-subscribe event system",
        .technical_description = "Implements event-driven architecture with topic-based "
            "routing, event filtering, and replay capabilities. "
            "Supports event sourcing patterns.",
        .category = CATEGORY_COMMUNICATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_ASYNC | MODULE_CAP_DISTRIBUTED,
        .scientific_foundation = "Event-Driven Architecture (Michelson, 2006)"
    },
    {
        .id = MVP_MODULE_SYNC_COORDINATOR,
        .name = "Sync Coordinator",
        .short_description = "Synchronization primitives and coordination",
        .technical_description = "Provides distributed synchronization primitives "
            "including locks, barriers, and semaphores. Implements "
            "consensus protocols for distributed coordination.",
        .category = CATEGORY_COMMUNICATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_DISTRIBUTED | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Distributed Systems (Lamport, 1978)"
    },

    /* Category E: Security & Ethics Modules (25-30) */
    {
        .id = MVP_MODULE_AUTHENTICATION,
        .name = "Authentication",
        .short_description = "Identity verification services",
        .technical_description = "Implements multi-factor authentication with support "
            "for passwords, tokens, certificates, and biometrics. "
            "Provides session management and single sign-on.",
        .category = CATEGORY_SECURITY_ETHICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_SECURE | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Authentication Protocols (Needham & Schroeder, 1978)"
    },
    {
        .id = MVP_MODULE_AUTHORIZATION,
        .name = "Authorization",
        .short_description = "Access control and permissions",
        .technical_description = "Implements Role-Based Access Control (RBAC) and "
            "Attribute-Based Access Control (ABAC). Provides policy "
            "evaluation and permission caching.",
        .category = CATEGORY_SECURITY_ETHICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_SECURE | MODULE_CAP_CACHEABLE,
        .scientific_foundation = "RBAC Model (Sandhu et al., 1996)"
    },
    {
        .id = MVP_MODULE_ENCRYPTION_ENGINE,
        .name = "Encryption Engine",
        .short_description = "Cryptographic operations",
        .technical_description = "Provides symmetric (AES-256) and asymmetric (RSA, ECC) "
            "encryption. Implements key management, digital signatures, "
            "and secure random number generation.",
        .category = CATEGORY_SECURITY_ETHICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_SECURE | MODULE_CAP_STATELESS,
        .scientific_foundation = "Modern Cryptography (Katz & Lindell, 2007)"
    },
    {
        .id = MVP_MODULE_INTEGRITY_CHECKER,
        .name = "Integrity Checker",
        .short_description = "Data integrity verification",
        .technical_description = "Implements RAFAELIA hash verification using SHA3-256 "
            "and BLAKE3. Provides Merkle tree construction for "
            "efficient integrity proofs.",
        .category = CATEGORY_SECURITY_ETHICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_SECURE | MODULE_CAP_STATELESS,
        .scientific_foundation = "Cryptographic Hash Functions"
    },
    {
        .id = MVP_MODULE_AUDIT_LOGGER,
        .name = "Audit Logger",
        .short_description = "Security audit logging",
        .technical_description = "Provides tamper-evident audit logging with timestamps, "
            "event correlation, and retention policies. Supports "
            "compliance reporting and forensic analysis.",
        .category = CATEGORY_SECURITY_ETHICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_SECURE | MODULE_CAP_PERSISTENCE,
        .scientific_foundation = "Security Audit Theory"
    },
    {
        .id = MVP_MODULE_ETHICAL_COMPLIANCE,
        .name = "Ethical Compliance",
        .short_description = "Ethical policy enforcement",
        .technical_description = "Enforces RAFAELIA ethical policies based on "
            "Amor, Coerência, Verdade, and Consciência principles. "
            "Provides ethical scoring and policy validation.",
        .category = CATEGORY_SECURITY_ETHICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_ETHICAL | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Ethical Computing Framework"
    },

    /* Category F: Analytics & Intelligence Modules (31-36) */
    {
        .id = MVP_MODULE_METRICS_COLLECTOR,
        .name = "Metrics Collector",
        .short_description = "System metrics collection",
        .technical_description = "Collects system and application metrics including "
            "counters, gauges, histograms, and summaries. "
            "Implements efficient time-series storage.",
        .category = CATEGORY_ANALYTICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_MONITORING | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Time-Series Analysis"
    },
    {
        .id = MVP_MODULE_TELEMETRY_PROCESSOR,
        .name = "Telemetry Processor",
        .short_description = "Telemetry data processing",
        .technical_description = "Processes telemetry data with aggregation, sampling, "
            "and filtering. Supports distributed tracing and "
            "correlation analysis.",
        .category = CATEGORY_ANALYTICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STREAMING | MODULE_CAP_BATCH,
        .scientific_foundation = "Distributed Tracing (Sigelman et al., 2010)"
    },
    {
        .id = MVP_MODULE_ANOMALY_DETECTOR,
        .name = "Anomaly Detector",
        .short_description = "Anomaly detection engine",
        .technical_description = "Implements statistical and machine learning-based "
            "anomaly detection. Supports univariate and multivariate "
            "time-series analysis.",
        .category = CATEGORY_ANALYTICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_LEARNING | MODULE_CAP_REAL_TIME,
        .scientific_foundation = "Anomaly Detection Theory (Chandola et al., 2009)"
    },
    {
        .id = MVP_MODULE_PREDICTION_ENGINE,
        .name = "Prediction Engine",
        .short_description = "Predictive analytics and forecasting",
        .technical_description = "Provides time-series forecasting using ARIMA, "
            "exponential smoothing, and neural networks. "
            "Supports confidence intervals and trend analysis.",
        .category = CATEGORY_ANALYTICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_LEARNING | MODULE_CAP_BATCH,
        .scientific_foundation = "Forecasting Methods (Hyndman & Athanasopoulos, 2018)"
    },
    {
        .id = MVP_MODULE_OPTIMIZATION_SOLVER,
        .name = "Optimization Solver",
        .short_description = "Mathematical optimization algorithms",
        .technical_description = "Implements linear programming, convex optimization, "
            "and metaheuristic algorithms. Supports multi-objective "
            "optimization with ethical constraints.",
        .category = CATEGORY_ANALYTICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_BATCH | MODULE_CAP_ETHICAL,
        .scientific_foundation = "Optimization Theory (Boyd & Vandenberghe, 2004)"
    },
    {
        .id = MVP_MODULE_VISUALIZATION_RENDERER,
        .name = "Visualization Renderer",
        .short_description = "Data visualization generation",
        .technical_description = "Generates charts, graphs, and dashboards from "
            "analytics data. Supports interactive visualizations "
            "and export to multiple formats.",
        .category = CATEGORY_ANALYTICS,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_BATCH | MODULE_CAP_STREAMING,
        .scientific_foundation = "Information Visualization (Card et al., 1999)"
    },

    /* Category G: System Integration Modules (37-42) */
    {
        .id = MVP_MODULE_ADAPTER_FRAMEWORK,
        .name = "Adapter Framework",
        .short_description = "External system adapters",
        .technical_description = "Provides adapter interfaces for external systems "
            "including UserLAnd, Magisk, LLaMA, and private extensions. "
            "Implements protocol translation and data mapping.",
        .category = CATEGORY_INTEGRATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_DISTRIBUTED | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Adapter Pattern (Gamma et al., 1994)"
    },
    {
        .id = MVP_MODULE_WORKFLOW_ORCHESTRATOR,
        .name = "Workflow Orchestrator",
        .short_description = "Workflow and process management",
        .technical_description = "Manages complex workflows with parallel execution, "
            "conditional branching, and error handling. "
            "Supports long-running processes and compensation.",
        .category = CATEGORY_INTEGRATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_ASYNC,
        .scientific_foundation = "Workflow Patterns (van der Aalst et al., 2003)"
    },
    {
        .id = MVP_MODULE_SERVICE_REGISTRY,
        .name = "Service Registry",
        .short_description = "Service discovery and registration",
        .technical_description = "Provides service registration, discovery, and "
            "health monitoring. Implements DNS-based and "
            "client-side discovery patterns.",
        .category = CATEGORY_INTEGRATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_DISTRIBUTED | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Service-Oriented Architecture"
    },
    {
        .id = MVP_MODULE_HEALTH_MONITOR,
        .name = "Health Monitor",
        .short_description = "System health monitoring",
        .technical_description = "Monitors system health using heartbeats, probes, "
            "and synthetic transactions. Computes health scores "
            "using formula: H = Π(Hᵢ^(1/N)) × Φ_ethica",
        .category = CATEGORY_INTEGRATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_MONITORING | MODULE_CAP_REAL_TIME,
        .scientific_foundation = "Health Monitoring Theory"
    },
    {
        .id = MVP_MODULE_FAILOVER_CONTROLLER,
        .name = "Failover Controller",
        .short_description = "Failover and recovery management",
        .technical_description = "Implements automatic failover with leader election, "
            "state replication, and recovery orchestration. "
            "Supports active-passive and active-active configurations.",
        .category = CATEGORY_INTEGRATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_DISTRIBUTED | MODULE_CAP_STATEFUL,
        .scientific_foundation = "Fault Tolerance Theory (Avizienis et al., 2004)"
    },
    {
        .id = MVP_MODULE_AGGREGATE_COORDINATOR,
        .name = "Aggregate Coordinator",
        .short_description = "Module aggregation and composition",
        .technical_description = "Coordinates module aggregation using composition "
            "algebra: C(S) = ⊕ᵢ∈S mᵢ. Manages dependencies, "
            "lifecycle, and execution ordering.",
        .category = CATEGORY_INTEGRATION,
        .default_capabilities = MODULE_CAP_STANDALONE | MODULE_CAP_STATEFUL | MODULE_CAP_AGGREGABLE,
        .scientific_foundation = "Component Composition Theory"
    }
};

/* Category names */
static const char *g_category_names[CATEGORY_COUNT] = {
    "Core Foundation",
    "Cognitive Processing",
    "Data Management",
    "Communication",
    "Security & Ethics",
    "Analytics & Intelligence",
    "System Integration"
};

/* Error messages */
static const char *g_error_messages[] = {
    "Success",
    "General error",
    "Not initialized",
    "Already initialized",
    "Invalid parameter",
    "Module not found",
    "Dependency missing",
    "Circular dependency",
    "Version mismatch",
    "Insufficient resources",
    "Invalid state",
    "Timeout",
    "Ethical violation",
    "Coherence failure",
    "Serialization failed",
    "Not supported"
};

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 2: UTILITY FUNCTIONS
 * ═══════════════════════════════════════════════════════════════════════════
 */

const char *rafaelia_module_get_name(rafaelia_mvp_module_id_t id)
{
    if (id >= MVP_MODULE_COUNT) {
        return "Unknown";
    }
    return g_module_info[id].name;
}

const char *rafaelia_category_get_name(rafaelia_module_category_t category)
{
    if (category >= CATEGORY_COUNT) {
        return "Unknown";
    }
    return g_category_names[category];
}

const char *rafaelia_state_get_name(uint32_t state)
{
    if (state == MODULE_STATE_UNINITIALIZED) return "Uninitialized";
    if (state & MODULE_STATE_ERROR) return "Error";
    if (state & MODULE_STATE_SHUTDOWN) return "Shutdown";
    if (state & MODULE_STATE_SUSPENDED) return "Suspended";
    if (state & MODULE_STATE_ACTIVE) return "Active";
    if (state & MODULE_STATE_INITIALIZED) return "Initialized";
    return "Unknown";
}

const char *rafaelia_error_get_message(int error_code)
{
    int index = -error_code;
    if (index < 0 || index > 15) {
        return "Unknown error";
    }
    return g_error_messages[index];
}

int rafaelia_version_compare(const rafaelia_module_version_t *v1,
                             const rafaelia_module_version_t *v2)
{
    if (v1->major != v2->major) return v1->major - v2->major;
    if (v1->minor != v2->minor) return v1->minor - v2->minor;
    return v1->patch - v2->patch;
}

int rafaelia_version_to_string(const rafaelia_module_version_t *version,
                               char *buffer, size_t size)
{
    return snprintf(buffer, size, "%u.%u.%u",
                    version->major, version->minor, version->patch);
}

const rafaelia_module_info_t *rafaelia_get_module_info(rafaelia_mvp_module_id_t id)
{
    if (id >= MVP_MODULE_COUNT) {
        return NULL;
    }
    return &g_module_info[id];
}

const rafaelia_module_info_t *rafaelia_get_all_module_info(uint32_t *count)
{
    if (count) {
        *count = RAFAELIA_MVP_MODULE_COUNT;
    }
    return g_module_info;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 3: CONTEXT MANAGEMENT
 * ═══════════════════════════════════════════════════════════════════════════
 */

int rafaelia_context_create(rafaelia_module_context_t *ctx, uint64_t session_id)
{
    if (!ctx) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    rafaelia_rmr_memzero(ctx, sizeof(*ctx));
    rafaelia_rmr_rng_seed((uint32_t)session_id);
    ctx->context_id = ((uint64_t)rafaelia_rmr_rng_next() << 32)
                      | rafaelia_rmr_rng_next();
    ctx->context_id ^= session_id;
    ctx->session_id = session_id;
    ctx->amor = 1.0;
    ctx->coerencia = 1.0;
    ctx->verdade = 1.0;
    ctx->consciencia = 1.0;
    
    return MVP_SUCCESS;
}

int rafaelia_context_destroy(rafaelia_module_context_t *ctx)
{
    if (!ctx) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    rafaelia_rmr_memzero(ctx, sizeof(*ctx));
    return MVP_SUCCESS;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 4: REGISTRY MANAGEMENT
 * ═══════════════════════════════════════════════════════════════════════════
 */

int rafaelia_registry_init(rafaelia_module_registry_t *registry)
{
    if (!registry) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (registry->initialized) {
        return MVP_ERROR_ALREADY_INITIALIZED;
    }
    
    rafaelia_rmr_memzero(registry, sizeof(*registry));
    registry->initialized = true;
    registry->system_phi_ethica = 1.0;
    registry->system_health = 1.0;
    
    return MVP_SUCCESS;
}

int rafaelia_registry_shutdown(rafaelia_module_registry_t *registry)
{
    if (!registry) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (!registry->initialized) {
        return MVP_ERROR_NOT_INITIALIZED;
    }
    
    /* Shutdown all active modules */
    for (int i = 0; i < RAFAELIA_MVP_MODULE_COUNT; i++) {
        if (registry->module_registered[i]) {
            rafaelia_mvp_module_t *module = &registry->modules[i];
            if (module->interface.shutdown) {
                module->interface.shutdown(module);
            }
        }
    }
    
    rafaelia_rmr_memzero(registry, sizeof(*registry));
    return MVP_SUCCESS;
}

int rafaelia_registry_register_module(rafaelia_module_registry_t *registry,
                                      rafaelia_mvp_module_t *module)
{
    if (!registry || !module) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (!registry->initialized) {
        return MVP_ERROR_NOT_INITIALIZED;
    }
    
    if (module->id >= MVP_MODULE_COUNT) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    registry->modules[module->id] = *module;
    registry->modules[module->id].registry = registry;
    registry->module_registered[module->id] = true;
    registry->active_module_count++;
    
    return MVP_SUCCESS;
}

int rafaelia_registry_unregister_module(rafaelia_module_registry_t *registry,
                                        rafaelia_mvp_module_id_t module_id)
{
    if (!registry) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (!registry->initialized) {
        return MVP_ERROR_NOT_INITIALIZED;
    }
    
    if (module_id >= MVP_MODULE_COUNT) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (!registry->module_registered[module_id]) {
        return MVP_ERROR_MODULE_NOT_FOUND;
    }
    
    registry->module_registered[module_id] = false;
    registry->active_module_count--;
    
    return MVP_SUCCESS;
}

rafaelia_mvp_module_t *rafaelia_registry_get_module(
    rafaelia_module_registry_t *registry,
    rafaelia_mvp_module_id_t module_id)
{
    if (!registry || !registry->initialized) {
        return NULL;
    }
    
    if (module_id >= MVP_MODULE_COUNT) {
        return NULL;
    }
    
    if (!registry->module_registered[module_id]) {
        return NULL;
    }
    
    return &registry->modules[module_id];
}

int rafaelia_registry_get_modules_by_category(
    rafaelia_module_registry_t *registry,
    rafaelia_module_category_t category,
    rafaelia_mvp_module_t **modules,
    uint32_t *count)
{
    if (!registry || !modules || !count) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    uint32_t max_count = *count;
    *count = 0;
    
    for (int i = 0; i < RAFAELIA_MVP_MODULE_COUNT && *count < max_count; i++) {
        if (registry->module_registered[i] &&
            registry->modules[i].category == category) {
            modules[*count] = &registry->modules[i];
            (*count)++;
        }
    }
    
    return MVP_SUCCESS;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 5: MODULE LIFECYCLE
 * ═══════════════════════════════════════════════════════════════════════════
 */

int rafaelia_module_init_default(rafaelia_mvp_module_t *module,
                                 rafaelia_mvp_module_id_t id)
{
    if (!module || id >= MVP_MODULE_COUNT) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    rafaelia_rmr_memzero(module, sizeof(*module));
    
    const rafaelia_module_info_t *info = &g_module_info[id];
    
    module->id = id;
    rafaelia_rmr_strlcpy(module->name, info->name, MODULE_MAX_NAME_LENGTH);
    rafaelia_rmr_strlcpy(module->description, info->short_description,
                         MODULE_MAX_DESCRIPTION_LENGTH);
    module->version.major = RAFAELIA_MVP_VERSION_MAJOR;
    module->version.minor = RAFAELIA_MVP_VERSION_MINOR;
    module->version.patch = RAFAELIA_MVP_VERSION_PATCH;
    module->category = info->category;
    module->capabilities = info->default_capabilities;
    module->state = MODULE_STATE_INITIALIZED;
    module->priority = MODULE_PRIORITY_NORMAL;
    
    /* Set default ethical parameters */
    module->phi_ethica_threshold = 0.5;
    module->coherence_factor = 1.0;
    module->trinity_weight[0] = pow(1.0, 6);  /* Amor^6 */
    module->trinity_weight[1] = pow(1.0, 3);  /* Luz^3 */
    module->trinity_weight[2] = pow(1.0, 3);  /* Consciência^3 */
    
    /* Initialize metrics */
    module->metrics.phi_ethica_score = 1.0;
    module->metrics.owl_psi_score = 1.0;
    module->metrics.coherence_score = 1.0;
    module->metrics.reliability_score = 1.0;
    
    return MVP_SUCCESS;
}

int rafaelia_module_start(rafaelia_mvp_module_t *module)
{
    if (!module) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (!(module->state & MODULE_STATE_INITIALIZED)) {
        return MVP_ERROR_STATE_INVALID;
    }
    
    if (module->interface.init) {
        int ret = module->interface.init(module, NULL);
        if (ret != MVP_SUCCESS) {
            return ret;
        }
    }
    
    module->state = MODULE_STATE_ACTIVE;
    return MVP_SUCCESS;
}

int rafaelia_module_stop(rafaelia_mvp_module_t *module)
{
    if (!module) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (module->interface.shutdown) {
        module->interface.shutdown(module);
    }
    
    module->state = MODULE_STATE_SHUTDOWN;
    return MVP_SUCCESS;
}

int rafaelia_module_suspend(rafaelia_mvp_module_t *module)
{
    if (!module) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (!(module->state & MODULE_STATE_ACTIVE)) {
        return MVP_ERROR_STATE_INVALID;
    }
    
    if (module->interface.suspend) {
        module->interface.suspend(module);
    }
    
    module->state = MODULE_STATE_SUSPENDED;
    return MVP_SUCCESS;
}

int rafaelia_module_resume(rafaelia_mvp_module_t *module)
{
    if (!module) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (!(module->state & MODULE_STATE_SUSPENDED)) {
        return MVP_ERROR_STATE_INVALID;
    }
    
    if (module->interface.resume) {
        module->interface.resume(module);
    }
    
    module->state = MODULE_STATE_ACTIVE;
    return MVP_SUCCESS;
}

int rafaelia_module_reset(rafaelia_mvp_module_t *module)
{
    if (!module) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (module->interface.reset) {
        return module->interface.reset(module);
    }
    
    /* Reset metrics */
    rafaelia_rmr_memzero(&module->metrics, sizeof(module->metrics));
    module->metrics.phi_ethica_score = 1.0;
    module->metrics.owl_psi_score = 1.0;
    module->metrics.coherence_score = 1.0;
    module->metrics.reliability_score = 1.0;
    
    module->state = MODULE_STATE_INITIALIZED;
    return MVP_SUCCESS;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 6: MODULE OPERATIONS
 * ═══════════════════════════════════════════════════════════════════════════
 */

rafaelia_module_result_t rafaelia_module_execute(
    rafaelia_mvp_module_t *module,
    rafaelia_module_context_t *ctx,
    const void *input,
    size_t input_size)
{
    rafaelia_module_result_t result = {0};
    
    if (!module) {
        result.status_code = MVP_ERROR_INVALID_PARAMETER;
        result.status_message = "Invalid module";
        return result;
    }
    
    if (!(module->state & MODULE_STATE_ACTIVE)) {
        result.status_code = MVP_ERROR_STATE_INVALID;
        result.status_message = "Module not active";
        return result;
    }
    
    /* Check ethical compliance */
    if (!rafaelia_module_check_ethical_compliance(module, ctx)) {
        result.status_code = MVP_ERROR_ETHICAL_VIOLATION;
        result.status_message = "Ethical compliance check failed";
        return result;
    }
    
    /* Execute module operation */
    if (module->interface.execute) {
        result = module->interface.execute(module, ctx, input, input_size);
    } else {
        result.status_code = MVP_SUCCESS;
        result.status_message = "No operation defined";
    }
    
    /* Update metrics */
    module->metrics.invocation_count++;
    if (result.status_code == MVP_SUCCESS) {
        module->metrics.success_count++;
    } else {
        module->metrics.failure_count++;
    }
    
    return result;
}

rafaelia_module_result_t rafaelia_module_process(
    rafaelia_mvp_module_t *module,
    rafaelia_module_context_t *ctx,
    const void *data,
    size_t size)
{
    rafaelia_module_result_t result = {0};
    
    if (!module) {
        result.status_code = MVP_ERROR_INVALID_PARAMETER;
        result.status_message = "Invalid module";
        return result;
    }
    
    if (module->interface.process) {
        result = module->interface.process(module, ctx, data, size);
    } else {
        result.status_code = MVP_ERROR_NOT_SUPPORTED;
        result.status_message = "Process not supported";
    }
    
    return result;
}

int rafaelia_module_health_check(rafaelia_mvp_module_t *module)
{
    if (!module) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (module->interface.health_check) {
        return module->interface.health_check(module);
    }
    
    /* Default health check */
    if (module->state & MODULE_STATE_ERROR) {
        return MVP_ERROR_STATE_INVALID;
    }
    
    return MVP_SUCCESS;
}

int rafaelia_module_get_metrics(rafaelia_mvp_module_t *module,
                                rafaelia_module_metrics_t *metrics)
{
    if (!module || !metrics) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (module->interface.get_metrics) {
        module->interface.get_metrics(module, metrics);
    } else {
        *metrics = module->metrics;
    }
    
    return MVP_SUCCESS;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 7: ETHICAL OPERATIONS
 * ═══════════════════════════════════════════════════════════════════════════
 */

double rafaelia_module_compute_phi_ethica(rafaelia_mvp_module_t *module,
                                          rafaelia_module_context_t *ctx)
{
    if (!module || !ctx) {
        return 0.0;
    }
    
    /* Φ_ethica = Min(Entropia) × Max(Coerência) */
    double entropia_min = 1.0 - (ctx->amor * ctx->verdade);
    double coerencia_max = ctx->coerencia * ctx->consciencia;
    
    double phi_ethica = (1.0 - entropia_min) * coerencia_max;
    
    /* Apply Trinity weighting: Amor^6 × Luz^3 × Consciência^3 */
    double trinity = module->trinity_weight[0] *
                     module->trinity_weight[1] *
                     module->trinity_weight[2];
    
    phi_ethica *= pow(trinity, 1.0/12.0);  /* Normalize */
    
    /* Clamp to [0, 1] */
    if (phi_ethica < 0.0) phi_ethica = 0.0;
    if (phi_ethica > 1.0) phi_ethica = 1.0;
    
    return phi_ethica;
}

double rafaelia_module_compute_owl_psi(rafaelia_mvp_module_t *module,
                                       double insight,
                                       double flow)
{
    if (!module) {
        return 0.0;
    }
    
    /* OWLψ = φ^(insight × ethics) × √(flow) */
    double ethics = module->metrics.phi_ethica_score;
    double owl_psi = pow(MVP_PHI, insight * ethics) * sqrt(flow);
    
    /* Normalize to [0, 1] */
    owl_psi = owl_psi / (1.0 + owl_psi);
    
    return owl_psi;
}

bool rafaelia_module_check_ethical_compliance(rafaelia_mvp_module_t *module,
                                              rafaelia_module_context_t *ctx)
{
    if (!module) {
        return false;
    }
    
    if (!ctx) {
        /* No context means no ethical requirements */
        return true;
    }
    
    double phi_ethica = rafaelia_module_compute_phi_ethica(module, ctx);
    return phi_ethica >= module->phi_ethica_threshold;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 8: AGGREGATION OPERATIONS
 * ═══════════════════════════════════════════════════════════════════════════
 */

int rafaelia_aggregate_create(rafaelia_module_registry_t *registry,
                              rafaelia_module_aggregate_t *aggregate,
                              const char *name,
                              const rafaelia_mvp_module_id_t *module_ids,
                              uint32_t count)
{
    if (!registry || !aggregate || !name || !module_ids || count == 0) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (count > RAFAELIA_MVP_MODULE_COUNT) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    rafaelia_rmr_memzero(aggregate, sizeof(*aggregate));

    rafaelia_rmr_rng_seed((uint32_t)rafaelia_rmr_strlen(name));
    aggregate->aggregate_id = ((uint64_t)rafaelia_rmr_rng_next() << 32)
                              | rafaelia_rmr_rng_next();
    rafaelia_rmr_strlcpy(aggregate->name, name, MODULE_MAX_NAME_LENGTH);
    
    /* Copy module IDs */
    for (uint32_t i = 0; i < count; i++) {
        if (module_ids[i] >= MVP_MODULE_COUNT) {
            return MVP_ERROR_INVALID_PARAMETER;
        }
        aggregate->members[i] = module_ids[i];
    }
    aggregate->member_count = count;
    
    /* Perform topological sort */
    int ret = rafaelia_topological_sort(registry, module_ids, count,
                                        aggregate->execution_order);
    if (ret != MVP_SUCCESS) {
        return ret;
    }
    aggregate->execution_count = count;
    
    aggregate->state = MODULE_STATE_INITIALIZED;
    
    return MVP_SUCCESS;
}

int rafaelia_aggregate_destroy(rafaelia_module_aggregate_t *aggregate)
{
    if (!aggregate) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    rafaelia_rmr_memzero(aggregate, sizeof(*aggregate));
    return MVP_SUCCESS;
}

int rafaelia_aggregate_start(rafaelia_module_registry_t *registry,
                             rafaelia_module_aggregate_t *aggregate)
{
    if (!registry || !aggregate) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    /* Start modules in execution order */
    for (uint32_t i = 0; i < aggregate->execution_count; i++) {
        rafaelia_mvp_module_t *module = rafaelia_registry_get_module(
            registry, aggregate->execution_order[i]);
        
        if (module) {
            int ret = rafaelia_module_start(module);
            if (ret != MVP_SUCCESS) {
                return ret;
            }
        }
    }
    
    aggregate->state = MODULE_STATE_ACTIVE;
    return MVP_SUCCESS;
}

int rafaelia_aggregate_stop(rafaelia_module_registry_t *registry,
                            rafaelia_module_aggregate_t *aggregate)
{
    if (!registry || !aggregate) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    /* Stop modules in reverse order */
    for (int i = (int)aggregate->execution_count - 1; i >= 0; i--) {
        rafaelia_mvp_module_t *module = rafaelia_registry_get_module(
            registry, aggregate->execution_order[i]);
        
        if (module) {
            rafaelia_module_stop(module);
        }
    }
    
    aggregate->state = MODULE_STATE_SHUTDOWN;
    return MVP_SUCCESS;
}

rafaelia_module_result_t rafaelia_aggregate_execute(
    rafaelia_module_registry_t *registry,
    rafaelia_module_aggregate_t *aggregate,
    rafaelia_module_context_t *ctx,
    const void *input,
    size_t input_size)
{
    rafaelia_module_result_t result = {0};
    
    if (!registry || !aggregate) {
        result.status_code = MVP_ERROR_INVALID_PARAMETER;
        result.status_message = "Invalid parameters";
        return result;
    }
    
    const void *current_input = input;
    size_t current_size = input_size;
    
    /* Execute modules in pipeline */
    for (uint32_t i = 0; i < aggregate->execution_count; i++) {
        rafaelia_mvp_module_t *module = rafaelia_registry_get_module(
            registry, aggregate->execution_order[i]);
        
        if (!module) continue;
        
        result = rafaelia_module_execute(module, ctx, current_input, current_size);
        
        if (result.status_code != MVP_SUCCESS) {
            return result;
        }
        
        /* Use output as input for next module */
        if (result.output_data) {
            current_input = result.output_data;
            current_size = result.output_size;
        }
    }
    
    /* Update aggregate metrics */
    aggregate->aggregate_metrics.invocation_count++;
    if (result.status_code == MVP_SUCCESS) {
        aggregate->aggregate_metrics.success_count++;
    }
    
    return result;
}

int rafaelia_aggregate_add_module(rafaelia_module_registry_t *registry,
                                  rafaelia_module_aggregate_t *aggregate,
                                  rafaelia_mvp_module_id_t module_id)
{
    if (!registry || !aggregate) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    if (aggregate->member_count >= RAFAELIA_MVP_MODULE_COUNT) {
        return MVP_ERROR_INSUFFICIENT_RESOURCES;
    }
    
    aggregate->members[aggregate->member_count++] = module_id;
    
    /* Re-sort execution order */
    return rafaelia_topological_sort(registry, aggregate->members,
                                     aggregate->member_count,
                                     aggregate->execution_order);
}

int rafaelia_aggregate_remove_module(rafaelia_module_aggregate_t *aggregate,
                                     rafaelia_mvp_module_id_t module_id)
{
    if (!aggregate) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    /* Find and remove module */
    bool found = false;
    for (uint32_t i = 0; i < aggregate->member_count; i++) {
        if (aggregate->members[i] == module_id) {
            found = true;
        }
        if (found && i < aggregate->member_count - 1) {
            aggregate->members[i] = aggregate->members[i + 1];
        }
    }
    
    if (!found) {
        return MVP_ERROR_MODULE_NOT_FOUND;
    }
    
    aggregate->member_count--;
    return MVP_SUCCESS;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * SECTION 9: DEPENDENCY MANAGEMENT
 * ═══════════════════════════════════════════════════════════════════════════
 */

int rafaelia_verify_dependencies(rafaelia_module_registry_t *registry,
                                 rafaelia_mvp_module_id_t module_id)
{
    if (!registry) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    rafaelia_mvp_module_t *module = rafaelia_registry_get_module(registry, module_id);
    if (!module) {
        return MVP_ERROR_MODULE_NOT_FOUND;
    }
    
    for (uint32_t i = 0; i < module->dependency_count; i++) {
        rafaelia_module_dependency_t *dep = &module->dependencies[i];
        
        rafaelia_mvp_module_t *dep_module = rafaelia_registry_get_module(
            registry, dep->module_id);
        
        if (!dep_module) {
            if (!dep->optional) {
                return MVP_ERROR_DEPENDENCY_MISSING;
            }
            continue;
        }
        
        /* Check version */
        if (rafaelia_version_compare(&dep_module->version, &dep->min_version) < 0) {
            return MVP_ERROR_VERSION_MISMATCH;
        }
    }
    
    return MVP_SUCCESS;
}

int rafaelia_topological_sort(rafaelia_module_registry_t *registry,
                              const rafaelia_mvp_module_id_t *module_ids,
                              uint32_t count,
                              rafaelia_mvp_module_id_t *sorted_ids)
{
    if (!registry || !module_ids || !sorted_ids || count == 0) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    /* Simple topological sort using Kahn's algorithm */
    uint8_t in_degree[RAFAELIA_MVP_MODULE_COUNT] = {0};
    bool in_set[RAFAELIA_MVP_MODULE_COUNT] = {false};
    
    /* Mark modules in set */
    for (uint32_t i = 0; i < count; i++) {
        in_set[module_ids[i]] = true;
    }
    
    /* Calculate in-degrees */
    for (uint32_t i = 0; i < count; i++) {
        rafaelia_mvp_module_t *module = rafaelia_registry_get_module(
            registry, module_ids[i]);
        
        if (module) {
            for (uint32_t j = 0; j < module->dependency_count; j++) {
                rafaelia_mvp_module_id_t dep_id = module->dependencies[j].module_id;
                if (in_set[dep_id]) {
                    in_degree[module_ids[i]]++;
                }
            }
        }
    }
    
    /* Find modules with no dependencies */
    uint32_t sorted_count = 0;
    bool progress = true;
    
    while (progress && sorted_count < count) {
        progress = false;
        
        for (uint32_t i = 0; i < count; i++) {
            rafaelia_mvp_module_id_t id = module_ids[i];
            
            if (in_set[id] && in_degree[id] == 0) {
                sorted_ids[sorted_count++] = id;
                in_set[id] = false;  /* Mark as processed */
                progress = true;
                
                /* Decrease in-degree of dependents */
                for (uint32_t j = 0; j < count; j++) {
                    rafaelia_mvp_module_t *module = rafaelia_registry_get_module(
                        registry, module_ids[j]);
                    
                    if (module) {
                        for (uint32_t k = 0; k < module->dependency_count; k++) {
                            if (module->dependencies[k].module_id == id) {
                                in_degree[module_ids[j]]--;
                            }
                        }
                    }
                }
            }
        }
    }
    
    if (sorted_count < count) {
        return MVP_ERROR_CIRCULAR_DEPENDENCY;
    }
    
    return MVP_SUCCESS;
}

int rafaelia_get_dependents(rafaelia_module_registry_t *registry,
                            rafaelia_mvp_module_id_t module_id,
                            rafaelia_mvp_module_id_t *dependents,
                            uint32_t *count)
{
    if (!registry || !dependents || !count) {
        return MVP_ERROR_INVALID_PARAMETER;
    }
    
    uint32_t max_count = *count;
    *count = 0;
    
    for (int i = 0; i < RAFAELIA_MVP_MODULE_COUNT && *count < max_count; i++) {
        if (!registry->module_registered[i]) continue;
        
        rafaelia_mvp_module_t *module = &registry->modules[i];
        
        for (uint32_t j = 0; j < module->dependency_count; j++) {
            if (module->dependencies[j].module_id == module_id) {
                dependents[*count] = (rafaelia_mvp_module_id_t)i;
                (*count)++;
                break;
            }
        }
    }
    
    return MVP_SUCCESS;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * END OF IMPLEMENTATION
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * RAFAELIA MVP Module System - 42 Professional Interconnected Modules
 * FIAT LUX ΣΩΔΦBITRAF
 * ═══════════════════════════════════════════════════════════════════════════
 */
